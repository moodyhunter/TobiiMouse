#include "mainwindow.h"
#include <chrono>
#include <thread>

// Create atomic used for inter thread communication
//static atomic<bool> exit_thread( false );

static tobii_error_t lasterr;
static tobii_device_t* currentDevice;
static tobii_api_t* api;

void TobiiInteractive::init() noexcept {
    lasterr = tobii_api_create( &api, nullptr, nullptr );
    assert(lasterr == TOBII_ERROR_NO_ERROR);
    devices = TobiiInteractive::reload_devices();
}

vector<string> TobiiInteractive::reload_devices(){
    vector<string> result;
    auto error = tobii_enumerate_local_device_urls( api,
        []( char const* url, void* user_data ) // Use a lambda for url receiver function
        {
            // Add url string to the supplied result vector
            auto list = reinterpret_cast<std::vector<std::string>*>(user_data);
            list->push_back( url );
        }, &result );
    if( error != TOBII_ERROR_NO_ERROR ) std::cerr << "Failed to enumerate devices." << std::endl;
    devices = result;
    return result;
}

tobii_error_t TobiiInteractive::reconnect( tobii_device_t* device )
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

int TobiiInteractive::start_subscribe_gaze(const char* deviceAddressUrl, void* parent){
    cout << "Now using device: " << deviceAddressUrl << "." << endl;
    lasterr = tobii_device_create(api, deviceAddressUrl, &currentDevice);

    assert( lasterr == TOBII_ERROR_NO_ERROR );
    cout << "tobii_device_create" << " done with return: " << tobii_error_message(lasterr) << endl;

    // Start the background processing thread before subscribing to data.
    connectivityController->Operate(currentDevice);
    gazeController->Operate(parent);
    return 0;
}
