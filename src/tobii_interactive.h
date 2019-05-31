#ifndef TOBIIINTERACTIVE_H
#define TOBIIINTERACTIVE_H
#include <vector>
#include <string>
#include <iostream>
#include "assert.h"
#include "tobiimouse.h"
#include "tobii/tobii.h"
#include "tobii/tobii_streams.h"
#include "qthread_controller/qthreadcontroller.h"
#include "qthread_controller/qthreadworker.h"

using namespace std;
using namespace QThreadController;

namespace TobiiInteractive {
    class connectivityWorker : public QThreadWorker { void doWork(void *data1, void* data2) override;  };
    class gazeWorker : public QThreadWorker { void doWork(void *data1, void* data2) override; };

    void init(void* parentWindow) noexcept;
    int start_subscribe_gaze(const char* deviceAddressUrl);
    int stop_subscribe_gaze();
    vector<string> reload_devices();
    void url_receiver( char const* url, void* user_data );
    tobii_error_t reconnect( tobii_device_t* device );

    static vector<string> devices;

    void HandleConnectivityCallback(void* data);
    void HandleGazeCallback(void* data);


};

#endif // TOBIIINTERACTIVE_H
