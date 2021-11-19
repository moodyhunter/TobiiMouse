#ifndef TOBIIINTERACTIVE_H
#define TOBIIINTERACTIVE_H
#include <vector>
#include <string>
#include <iostream>
#include "assert.h"
#include "tobii/tobii.h"
#include "tobii/tobii_streams.h"
#include "qthread_controller/qthreadcontroller.h"
#include "qthread_controller/qthreadworker.h"

//using namespace std;
using namespace QThreadController;

namespace TobiiInteractive
{
    // Flags
    extern bool isRunning;
    class GazePointWorker : public QThreadWorker
    {
            void doWork(void *data1) override;
    };

    void Initialise() noexcept;
    QStringList ReloadDevices();
    tobii_error_t ReconnectTobii(tobii_device_t *device);
    int StartSubscribeGaze(QString deviceUrl);
    int StopSubscribeGaze();
    // API Callback
    void HandleGazeCallback(void *data);
    void GazeCallback(tobii_gaze_point_t const *gaze_point, void *user_data);
};

#endif // TOBIIINTERACTIVE_H
