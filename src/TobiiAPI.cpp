#include "TobiiAPI.hpp"

#include <iostream>

using namespace TobiiMouse;

TobiiAPI::TobiiAPI(QObject *parent) : QObject(parent)
{
    const auto lasterr = tobii_api_create(&tobii_api, nullptr, nullptr);
    assert(lasterr == TOBII_ERROR_NO_ERROR);
}

TobiiAPI::~TobiiAPI()
{
    for (const auto &c : children())
    {
        if (const auto dev = qobject_cast<TobiiDevice *>(c))
        {
            dev->UnsubscribeGazeData();
            delete dev;
        }
    }
    tobii_api_destroy(tobii_api);
}

QStringList TobiiAPI::GetDevices()
{
    QStringList result;
    const auto accumulator = [](char const *url, void *user_data) { static_cast<QStringList *>(user_data)->append(QString(url)); };
    const auto error = tobii_enumerate_local_device_urls(tobii_api, accumulator, &result);

    if (error != TOBII_ERROR_NO_ERROR)
        std::cerr << "Failed to enumerate devices." << std::endl;

    return result;
}

tobii_error_t TobiiAPI::ReconnectTobii(tobii_device_t *device)
{
    std::cout << "Reconnecting..." << std::endl;

    // Try reconnecting for 10 seconds before giving up
    for (int i = 0; i < 40; ++i)
    {
        const auto error = tobii_device_reconnect(device);
        if (error != TOBII_ERROR_CONNECTION_FAILED)
            return error;

        QThread::msleep(250);
    }

    return TOBII_ERROR_CONNECTION_FAILED;
}

TobiiDevice *TobiiAPI::OpenDevice(const QString &deviceUrl)
{
    tobii_device_t *device_t = nullptr;
    std::cout << "Device: " << deviceUrl.toStdString() << "." << std::endl;
    const auto lasterr = tobii_device_create(tobii_api, deviceUrl.toStdString().c_str(), &device_t);
    std::cout << "tobii_device_create returns: " << tobii_error_message(lasterr) << std::endl;
    return new TobiiMouse::TobiiDevice(device_t, this);
}
