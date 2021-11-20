#pragma once

#include "GazeWorkerThread.hpp"
#include "tobii/tobii.h"

#include <QObject>

namespace TobiiMouse
{
    class TobiiDevice : public QObject
    {
        Q_OBJECT
      public:
        explicit TobiiDevice(tobii_device_t *device, QObject *parent = nullptr);
        virtual ~TobiiDevice();
        int SubscribeGazeData();
        int UnsubscribeGazeData();

      signals:
        void OnGazeDataReady(float x, float y);

      private:
        TobiiMouse::GazeDataWorkerThread *workerThread = nullptr;
        tobii_device_t *deviceHandle;
    };
} // namespace TobiiMouse
