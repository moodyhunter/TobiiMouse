#include "tobii_interactive.h"
#include <assert.h>
#include <tuple>
#include "mainwindow.h"
#include "mouse_integration.h"

using namespace TobiiInteractive;

static tobii_device_t *thisDevice;

void TobiiInteractive::GazePointWorker::doWork(void *data1)
{
    thisDevice = static_cast<tobii_device_t *>(data1);
    bool subscribed = false;

    while (isRunning) {
        // Do a timed blocking wait for new gaze data, will time out after some hundred milliseconds
        // I don't know why they have two different functions for Windows and Linux.....
        // See: tobii.h:131
#ifdef __WIN32
        auto error = tobii_wait_for_callbacks(nullptr, 1, &thisDevice);   //The nullptr is for &tobii_engine
#else
        auto error = tobii_wait_for_callbacks(1, &device);   //Linux API Provides no such tobii_engine implementation. AFAIK.
#endif

        if (error == TOBII_ERROR_TIMED_OUT) continue;

        if (error == TOBII_ERROR_CONNECTION_FAILED) {
            // Block here while attempting reconnect, if it fails, exit the thread
            error = ReconnectTobii(thisDevice);

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
        error = tobii_device_process_callbacks(thisDevice);

        if (error == TOBII_ERROR_CONNECTION_FAILED) {
            // Block here while attempting reconnect, if it fails, exit the thread
            error = ReconnectTobii(thisDevice);

            if (error != TOBII_ERROR_NO_ERROR) {
                std::cerr << "Connection was lost and reconnection failed." << std::endl;
                return;
            }

            continue;
        } else if (error != TOBII_ERROR_NO_ERROR) {
            std::cerr << "tobii_device_process_callbacks failed: " << tobii_error_message(error) << "." << std::endl;
            emit ResultReady(nullptr);
        } else {
            if (!subscribed) {
                error = tobii_gaze_point_subscribe(thisDevice, TobiiInteractive::GazeCallback, this);

                if (error != TOBII_ERROR_NO_ERROR) {
                    std::cerr << "Failed to subscribe to gaze stream." << tobii_error_message(error) << std::endl;
                } else {
                    subscribed = true;
                }
            }
        }
    }
}

void TobiiInteractive::GazeCallback(tobii_gaze_point_t const *gaze_point, void *user_data)
{
    if (isRunning) {
        auto _this = static_cast<QThreadWorker *>(user_data);

        if (gaze_point->validity == TOBII_VALIDITY_VALID) {
            auto results = make_tuple(gaze_point->position_xy[0], gaze_point->position_xy[1]);
            emit _this->ResultReady(&results);
        }
    }
}

void TobiiInteractive::HandleGazeCallback(void *data)
{
    auto data_collection = *static_cast<tuple<float, float>*>(data);
    auto x = get<0>(data_collection);
    auto y = get<1>(data_collection);
    MainWindow::instance->OnGazePositionUIUpdate(x, y);
    MouseIntegration::OnGaze(x, y);
}
