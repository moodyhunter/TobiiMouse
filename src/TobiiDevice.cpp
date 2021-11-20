#include "TobiiDevice.hpp"

#include <iostream>

using namespace TobiiMouse;

TobiiDevice::TobiiDevice(tobii_device_t *device, QObject *parent) : QObject(parent), deviceHandle(device)
{
    workerThread = new GazeDataWorkerThread(device, this);
    connect(workerThread, &GazeDataWorkerThread::OnGazePointReady, this, &TobiiDevice::OnGazeDataReady);
    connect(workerThread, &GazeDataWorkerThread::finished, workerThread, &QObject::deleteLater);
}

TobiiDevice::~TobiiDevice()
{
    if (workerThread->isRunning())
        UnsubscribeGazeData();
    tobii_device_destroy(deviceHandle);
}

int TobiiDevice::SubscribeGazeData()
{
    if (workerThread->isRunning())
        UnsubscribeGazeData();

    workerThread->start();
    return 0;
}

int TobiiDevice::UnsubscribeGazeData()
{
    workerThread->Stop();
    workerThread->wait();
    return 0;
}
