#pragma once

#include "tobii/tobii.h"

#include <QThread>

namespace TobiiMouse
{
    class GazeDataWorkerThread : public QThread
    {
        Q_OBJECT
      public:
        GazeDataWorkerThread(tobii_device_t *deviceHandle, QObject *parent = nullptr);
        void Stop();

      protected:
        void run() override;

      signals:
        void OnGazePointReady(float x, float y);

      private:
        bool stopFlag = false;
        tobii_device_t *thisDevice;
    };
} // namespace TobiiMouse::Gaze
