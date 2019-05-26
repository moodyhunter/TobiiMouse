#include <stdio.h>
#include <QString>
#include <list>
#include <vector>
#include <thread>
#include <iostream>
#include "assert.h"
#include "cstring"

#include "tobii/tobii.h"
#include "tobii/tobii_streams.h"
#include "tobii_interactive.h"

using namespace std;

TobiiInteractive::TobiiInteractive() noexcept {
    lasterr = tobii_api_create( &api, nullptr, nullptr );
    assert(lasterr == TOBII_ERROR_NO_ERROR);
    devices = list_devices();
}


static tobii_error_t reconnect( tobii_device_t* device )
{
    // Try reconnecting for 10 seconds before giving up
    for( int i = 0; i < 40; ++i )
    {
        auto error = tobii_device_reconnect( device );
        if( error != TOBII_ERROR_CONNECTION_FAILED ) return error;
        std::this_thread::sleep_for( std::chrono::milliseconds( 250 ) );
    }

    return TOBII_ERROR_CONNECTION_FAILED;
}


void TobiiInteractive::gaze_point_callback( tobii_gaze_point_t const* gaze_point, void* user_data )
{
    assert(user_data == nullptr);
    if( gaze_point->validity == TOBII_VALIDITY_VALID )
        cout << "Gaze point: " << gaze_point->position_xy[ 0 ] << gaze_point->position_xy[ 1 ] << endl;
//        printf( "Gaze point: %f, %f\n",
//                static_cast<double>(),
//                static_cast<double>();
}


vector<string> TobiiInteractive::list_devices()
{
    vector<string> result;
    auto error = tobii_enumerate_local_device_urls( api,
        []( char const* url, void* user_data ) // Use a lambda for url receiver function
        {
            // Add url string to the supplied result vector
            auto list = reinterpret_cast<std::vector<std::string>*>(user_data);
            list->push_back( url );
        }, &result );
    if( error != TOBII_ERROR_NO_ERROR ) std::cerr << "Failed to enumerate devices." << std::endl;

    return result;
}

int TobiiInteractive::testmain()
{
    const string first_address = devices.front();
    cout << first_address << endl;

    tobii_device_t* device;
    lasterr = tobii_device_create( api, first_address.c_str(), &device );

    //"tobii-ttp://IS404-100107046346"
    cout << first_address << endl;
    assert( lasterr == TOBII_ERROR_NO_ERROR );

    // Create atomic used for inter thread communication
    std::atomic<bool> exit_thread( false );

    // Start the background processing thread before subscribing to data
    std::thread thread(
        [&exit_thread, device]()
        {
            while( !exit_thread )
            {
                // Do a timed blocking wait for new gaze data, will time out after some hundred milliseconds
                auto error = tobii_wait_for_callbacks( nullptr, 1, &device );

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
                    return;
                }
            }
        } );
    // Start subscribing to gaze and supply lambda callback function to handle the gaze point data
    lasterr = tobii_gaze_point_subscribe( device,
        []( tobii_gaze_point_t const* gaze_point, void* user_data )
        {
            (void) user_data; // Unused parameter
            if( gaze_point->validity == TOBII_VALIDITY_VALID )
                std::cout << "Gaze point: " << gaze_point->timestamp_us << " " << gaze_point->position_xy[ 0 ]
                    << ", " << gaze_point->position_xy[ 1 ] << std::endl;
            else
                std::cout << "Gaze point: " << gaze_point->timestamp_us << " INVALID" << std::endl;
        }, nullptr );
    if( lasterr != TOBII_ERROR_NO_ERROR )
    {
        std::cerr << "Failed to subscribe to gaze stream." << std::endl;
        exit_thread = true;
        thread.join();
        tobii_device_destroy( device );
        tobii_api_destroy( api );
        return 1;
    }
}
