#include "GazeWorkerThread.hpp"

#include "TobiiAPI.hpp"
#include "tobii/tobii_streams.h"

#include <iostream>

using namespace TobiiMouse;

GazeDataWorkerThread::GazeDataWorkerThread(tobii_device_t *deviceHandle, QObject *parent) : QThread(parent), thisDevice(deviceHandle)
{
}

void GazeDataWorkerThread::run()
{
    bool subscribed = false;

    while (true)
    {
        // Do a timed blocking wait for new gaze data, will time out after some hundred milliseconds
        // I don't know why they have two different functions for Windows and Linux.....
        // See: tobii.h:131
#ifdef Q_OS_WIN
        auto error = tobii_wait_for_callbacks(nullptr, 1, &thisDevice); // The nullptr is for &tobii_engine
#else
        auto error = tobii_wait_for_callbacks(1, &thisDevice); // Linux API Provides no such tobii_engine implementation. AFAIK.
#endif

        if (error == TOBII_ERROR_TIMED_OUT)
            continue;

        //        if (error == TOBII_ERROR_CONNECTION_FAILED)
        //        {
        //            // Block here while attempting reconnect, if it fails, exit the thread
        //            error = ReconnectTobii(thisDevice);

        //            if (error != TOBII_ERROR_NO_ERROR)
        //            {
        //                std::cerr << "Connection was lost and reconnection failed." << std::endl;
        //                return;
        //            }

        //            continue;
        //        }
        //        else if (error != TOBII_ERROR_NO_ERROR)
        //        {
        //            std::cerr << "tobii_wait_for_callbacks failed: " << tobii_error_message(error) << "." << std::endl;
        //            return;
        //        }

        // Calling this function will execute the subscription callback functions
        error = tobii_device_process_callbacks(thisDevice);

        //        if (error == TOBII_ERROR_CONNECTION_FAILED)
        //        {
        //            // Block here while attempting reconnect, if it fails, exit the thread
        //            error = ReconnectTobii(thisDevice);

        //            if (error != TOBII_ERROR_NO_ERROR)
        //            {
        //                std::cerr << "Connection was lost and reconnection failed." << std::endl;
        //                return;
        //            }

        //            continue;
        //        }

        if (error != TOBII_ERROR_NO_ERROR)
        {
            std::cerr << "tobii_device_process_callbacks failed: " << tobii_error_message(error) << "." << std::endl;
            emit OnGazePointReady(0, 0);
            continue;
        }

        if (!subscribed)
        {
            const auto GazeCallback = [](tobii_gaze_point_t const *gaze_point, void *user_data)
            {
                auto _this = static_cast<GazeDataWorkerThread *>(user_data);
                if (gaze_point->validity == TOBII_VALIDITY_VALID)
                    emit _this->OnGazePointReady(gaze_point->position_xy[0], gaze_point->position_xy[1]);
            };

            error = tobii_gaze_point_subscribe(thisDevice, GazeCallback, this);

            if (error != TOBII_ERROR_NO_ERROR)
            {
                std::cerr << "Failed to subscribe to gaze stream." << tobii_error_message(error) << std::endl;
            }
            else
            {
                subscribed = true;
            }
        }
    }
}
