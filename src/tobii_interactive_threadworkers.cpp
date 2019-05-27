#include "tobii_interactive.h"
#include <assert.h>
#include "mainwindow.h"

static tobii_device_t* device;
static MainWindow* mainwindow;

void TobiiInteractive::connectivityWorker::doWork(void* data) {
    //assert(data == nullptr); // We don't want anything.
    device = reinterpret_cast<tobii_device_t*>(data);
    while( true /*!exit_thread */)
    {
        // Do a timed blocking wait for new gaze data, will time out after some hundred milliseconds

        // I don't know why they have two different functions for Windows and Linux.....
        // See: tobii.h:131
#ifdef __WIN32
        auto error = tobii_wait_for_callbacks( nullptr, 1, &device ); //The nullptr is for &tobii_engine
#else
        auto error = tobii_wait_for_callbacks( 1, &device ); //Linux API Provides no such tobii_engine implementation. AFAIK.
#endif
        if( error == TOBII_ERROR_TIMED_OUT ) continue; // If timed out, redo the wait for callbacks call
        if( error == TOBII_ERROR_CONNECTION_FAILED )
        {
                // Block here while attempting reconnect, if it fails, exit the thread
            error = reconnect( device );
            if( error != TOBII_ERROR_NO_ERROR )
            {
                    std::cerr << "Connection was lost and reconnection failed." << std::endl;
                    return;
            }
            continue;
        }
        else if( error != TOBII_ERROR_NO_ERROR )
        {
            std::cerr << "tobii_wait_for_callbacks failed: " << tobii_error_message( error ) << "." << std::endl;
            return;
        }
        // Calling this function will execute the subscription callback functions
        error = tobii_device_process_callbacks( device );
        if( error == TOBII_ERROR_CONNECTION_FAILED )
        {
            // Block here while attempting reconnect, if it fails, exit the thread
            error = reconnect( device );
            if( error != TOBII_ERROR_NO_ERROR )
            {
                std::cerr << "Connection was lost and reconnection failed." << std::endl;
                return;
            }
            continue;
        }
        else if( error != TOBII_ERROR_NO_ERROR )
        {
            std::cerr << "tobii_device_process_callbacks failed: " << tobii_error_message( error ) << "." << std::endl;
            emit ResultReady(nullptr);
        }
    }
}

void TobiiInteractive::gazeWorker::doWork(void* data){
    mainwindow = reinterpret_cast<MainWindow*>(data);
    // Start subscribing to gaze and supply lambda callback function to handle the gaze point data
    auto err = tobii_gaze_point_subscribe( device,
        []( tobii_gaze_point_t const* gaze_point, void* user_data )
        {
            gazeWorker* parent = reinterpret_cast<gazeWorker*>(user_data);
            if(gaze_point->validity == TOBII_VALIDITY_VALID)
            {
                emit parent->ResultReady((void *)gaze_point);
            }
        }, this );

    if( err != TOBII_ERROR_NO_ERROR )
    {
        std::cerr << "Failed to subscribe to gaze stream." << tobii_error_message(err) << std::endl;
    }
}

void TobiiInteractive::HandleConnectivityCallback(void* data){

}

void TobiiInteractive::HandleGazeCallback(void* data){
    auto xy = reinterpret_cast<tobii_gaze_point_t*>(data);
    mainwindow->OnGazePositionReceived(xy->position_xy[0],xy->position_xy[1]);
}
