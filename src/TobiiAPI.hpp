#pragma once

#include "TobiiDevice.hpp"
#include "assert.h"
#include "tobii/tobii.h"
#include "tobii/tobii_streams.h"

namespace TobiiMouse
{
    class TobiiAPI : public QObject
    {
        Q_OBJECT
      public:
        TobiiAPI(QObject *parent = nullptr);
        QStringList GetDevices();
        tobii_error_t ReconnectTobii(tobii_device_t *device);
        TobiiMouse::TobiiDevice *OpenDevice(const QString &deviceUrl);

        virtual ~TobiiAPI();

      private:
        tobii_api_t *tobii_api = nullptr;
    };

}; // namespace TobiiMouse
