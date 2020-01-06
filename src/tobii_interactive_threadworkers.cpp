#include "tobii_interactive.h"
#include <assert.h>
#include <tuple>
#include "mainwindow.h"
#include "mouse_integration.h"

using namespace TobiiInteractive;

static tobii_device_t *device;

void TobiiInteractive::connectivityWorker::doWork(void *data1, void *data2)
{
    device = static_cast<tobii_device_t *>(data1);
    auto enabled = static_cast<bool *>(data2);

    while (*enabled) {
        // Do a timed blocking wait for new gaze data, will time out after some hundred milliseconds
        // I don't know why they have two different functions for Windows and Linux.....
        // See: tobii.h:131
#ifdef __WIN32
        auto error = tobii_wait_for_callbacks(nullptr, 1, &device);   //The nullptr is for &tobii_engine
#else
        auto error = tobii_wait_for_callbacks(1, &device);   //Linux API Provides no such tobii_engine implementation. AFAIK.
#endif

        if (error == TOBII_ERROR_TIMED_OUT) continue;

        if (error == TOBII_ERROR_CONNECTION_FAILED) {
            // Block here while attempting reconnect, if it fails, exit the thread
            error = reconnect(device);

            if (error != TOBII_ERROR_NO_ERROR) {
                std::cerr << "Connection was lost and reconnection failed." << std::endl;
                return;
            }

            continue;
        } else if (error != TOBII_ERROR_NO_ERROR) {
            std::cerr << "tobii_wait_for_callbacks failed: " << tobii_error_message(error) << "." << std::endl;
            return;
        }

        // Calling this function will execute the subscription callback functions
        error = tobii_device_process_callbacks(device);

        if (error == TOBII_ERROR_CONNECTION_FAILED) {
            // Block here while attempting reconnect, if it fails, exit the thread
            error = reconnect(device);

            if (error != TOBII_ERROR_NO_ERROR) {
                std::cerr << "Connection was lost and reconnection failed." << std::endl;
                return;
            }

            continue;
        } else if (error != TOBII_ERROR_NO_ERROR) {
            std::cerr << "tobii_device_process_callbacks failed: " << tobii_error_message(error) << "." << std::endl;
            emit ResultReady(nullptr);
        }
    }
}

// ------------------------------------- GAZE ------------------------------------------------

void TobiiInteractive::gazeWorker::doWork(void *data1, void *data2)
{
    Q_UNUSED(data2)
    //&GazeEnabled, This (used in slot emitter)
    tuple<bool *, QThreadWorker *> pointers(static_cast<bool *>(data1), this);
    auto err = tobii_gaze_point_subscribe(device, TobiiInteractive::GazeCallback, &pointers);

    if (err != TOBII_ERROR_NO_ERROR) {
        std::cerr << "Failed to subscribe to gaze stream." << tobii_error_message(err) << std::endl;
    }
}

void TobiiInteractive::GazeCallback(tobii_gaze_point_t const *gaze_point, void *user_data)
{
    auto data_collection = *reinterpret_cast<tuple<bool *, QThreadWorker *>*>(user_data);
    auto enabled = get<0>(data_collection);

    if (*enabled && gaze_point->validity == TOBII_VALIDITY_VALID) {
#ifdef __linux__
        auto _this = get<1>(data_collection);
        tuple<float, float> results(gaze_point->position_xy[0], gaze_point->position_xy[1]);
        emit _this->ResultReady(&results);
#elif _WIN32
        // Workaround of some strange pointer bug...
        mwindow->OnGazePositionUIUpdate(gaze_point->position_xy[0], gaze_point->position_xy[1]);
        MouseIntegration::OnGaze(gaze_point->position_xy[0], gaze_point->position_xy[1]);
#endif
    }
}

void TobiiInteractive::HandleConnectivityCallback(void *data)
{
    assert(data == nullptr);
}

void TobiiInteractive::HandleGazeCallback(void *data)
{
    auto data_collection = reinterpret_cast<tuple<float, float>*>(data);
    auto x = get<0>(*data_collection);
    auto y = get<1>(*data_collection);
    MainWindow::instance->OnGazePositionUIUpdate(x, y);
    MouseIntegration::OnGaze(x, y);
}
