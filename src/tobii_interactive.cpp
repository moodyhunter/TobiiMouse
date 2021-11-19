#include "mainwindow.h"
#include <chrono>
#include <thread>
#include "mouse_integration.h"


namespace TobiiInteractive
{
    bool isRunning = false;
    static tobii_error_t lasterr;
    static tobii_api_t *api;
    static tobii_device_t *CurrentDevice;

    static QStringList devices;
    static ThreadController *gazeController;
}

void TobiiInteractive::Initialise() noexcept
{
    isRunning = false;
    lasterr = tobii_api_create(&api, nullptr, nullptr);
    assert(lasterr == TOBII_ERROR_NO_ERROR);
    devices = TobiiInteractive::ReloadDevices();
    //
    gazeController = new ThreadController(new GazePointWorker(), &HandleGazeCallback);
    //
    MouseIntegration::init();
}

QStringList TobiiInteractive::ReloadDevices()
{
    QStringList result;
    auto error = tobii_enumerate_local_device_urls(api,
    [](char const * url, void *user_data) {
        static_cast<QStringList *>(user_data)->append(QString(url));
    }, &result);

    if (error != TOBII_ERROR_NO_ERROR) std::cerr << "Failed to enumerate devices." << std::endl;

    devices = result;
    return result;
}

tobii_error_t TobiiInteractive::ReconnectTobii(tobii_device_t *device)
{
 std::   cout << "Reconnecting..." << std::endl;

    // Try reconnecting for 10 seconds before giving up
    for (int i = 0; i < 40; ++i) {
        auto error = tobii_device_reconnect(device);

        if (error != TOBII_ERROR_CONNECTION_FAILED) return error;

        QThread::msleep(250);
    }

    return TOBII_ERROR_CONNECTION_FAILED;
}

int TobiiInteractive::StartSubscribeGaze(QString deviceUrl)
{
    if (isRunning) {
        StopSubscribeGaze();
    }

    isRunning = true;
    std::cout << "Device: " << deviceUrl.toStdString() << "." << std::endl;
    lasterr = tobii_device_create(api, deviceUrl.toStdString().c_str(), &CurrentDevice);
    Q_ASSERT(lasterr == TOBII_ERROR_NO_ERROR);
    std::cout << "tobii_device_create returns: " << tobii_error_message(lasterr) << std::endl;
    //
    // Start the background processing thread before subscribing to data.
    gazeController->StartOperate(CurrentDevice);
    return 0;
}

int TobiiInteractive::StopSubscribeGaze()
{
    isRunning = false;
    gazeController->StopOperate();
    return 0;
}
