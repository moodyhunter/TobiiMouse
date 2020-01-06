#include "mainwindow.h"
#include <chrono>
#include <thread>
#include "mouse_integration.h"

// Create atomic used for inter thread communication
//static atomic<bool> exit_thread( false );

namespace TobiiInteractive
{
    static tobii_error_t lasterr;
    static tobii_api_t *api;
    static tobii_device_t *CurrentDevice;

    static bool GazeEnabled;
    static QStringList devices;
    static ThreadController *connectivityController;
    static ThreadController *gazeController;
}


void TobiiInteractive::init() noexcept
{
    lasterr = tobii_api_create(&api, nullptr, nullptr);
    assert(lasterr == TOBII_ERROR_NO_ERROR);
    devices = TobiiInteractive::reload_devices();
    //
    connectivityController = new ThreadController(new connectivityWorker(), &HandleConnectivityCallback);
    gazeController = new ThreadController(new gazeWorker(), &HandleGazeCallback);
    //
    MouseIntegration::init();
}

QStringList TobiiInteractive::reload_devices()
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

tobii_error_t TobiiInteractive::reconnect(tobii_device_t *device)
{
    // Try reconnecting for 10 seconds before giving up
    for (int i = 0; i < 40; ++i) {
        auto error = tobii_device_reconnect(device);

        if (error != TOBII_ERROR_CONNECTION_FAILED) return error;

        QThread::msleep(250);
    }

    return TOBII_ERROR_CONNECTION_FAILED;
}

int TobiiInteractive::start_subscribe_gaze(QString deviceUrl)
{
    GazeEnabled = true;
    cout << "Device: " << deviceUrl.toStdString() << "." << endl;
    lasterr = tobii_device_create(api, deviceUrl.toStdString().c_str(), &CurrentDevice);
    assert(lasterr == TOBII_ERROR_NO_ERROR);
    cout << "tobii_device_create returns: " << tobii_error_message(lasterr) << endl;
    //
    // Start the background processing thread before subscribing to data.
    connectivityController->StartOperate(CurrentDevice, &GazeEnabled);
    gazeController->StartOperate(&GazeEnabled, nullptr);
    return 0;
}

int TobiiInteractive::stop_subscribe_gaze()
{
    GazeEnabled = false;
    connectivityController->StopOperate();
    gazeController->StopOperate();
    return 0;
}
