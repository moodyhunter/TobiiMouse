#ifndef TOBIIINTERACTIVE_H
#define TOBIIINTERACTIVE_H
#include "tobii/tobii.h"
#include <vector>
#include <string>
#include "tobii/tobii_streams.h"

using namespace std;

class TobiiInteractive {
public:
    explicit TobiiInteractive() noexcept;
    int start_subscribe_gaze(const char* deviceAddressUrl);
    void reload_devices();
    vector<string> devices;

private:
    static void gaze_point_callback(tobii_gaze_point_t const* gaze_point, void* user_data);
    static void url_receiver( char const* url, void* user_data );
    tobii_api_t *api;
    tobii_device_t* currentDevice;
    vector<string> list_devices();
    tobii_error_t lasterr;
};

#endif // TOBIIINTERACTIVE_H
