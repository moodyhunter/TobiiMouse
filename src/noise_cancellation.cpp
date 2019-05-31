/*
 * This is an example that demonstrates how to connect to the EyeX Engine and subscribe to the lightly filtered gaze data stream.
 *
 * Copyright 2013-2014 Tobii Technology AB. All rights reserved.
 * Copyright 2019 Leroy.H.Y (lhy20010403@hotmail.com).
 */

#include "noise_cancellation.h"

namespace NoiseCancellation {
static tuple<float, float> lastdata;

static constexpr const float FDeadZone = 8.0;       // pixels
static constexpr const float g_SlowZone = 40;       // pixels
static constexpr const float FSpeed = 0.5f;         // how fast to move when far away
static constexpr const float FSlowSpeed = 0.04f;    // now fast to move when close

static constexpr const float FIntegrationDeadZone = 1.0; //pixels
static constexpr const float integratingSpeed = 0.2f;

static float lastRawX;
static float lastRawY;

static float filteredX;
static float filteredY;

static float integratedErrorX;
static float integratedErrorY;

static float filteredErrorX; // this will be the distance between the gaze data and the current mouse position
static float filteredErrorY; // this will be the distance between the gaze data and the current mouse position
}

void NoiseCancellation::init(){
    lastRawX = 0;
    lastRawY = 0;

    filteredX = 0;
    filteredY = 0;

    integratedErrorX = 0;
    integratedErrorY = 0;

    filteredErrorX = 0; // this will be the distance between the gaze data and the current mouse position
    filteredErrorY = 0; // this will be the distance between the gaze data and the current mouse position
}

tuple<float, float> NoiseCancellation::CancelNoise(float x, float y)
{
    if (pow(fabs(lastRawX - x), 2) + pow(fabs(lastRawY - y), 2) > 320)
    {
        lastRawX = x;
        lastRawY = y;
        return lastdata; //reject noise
    }

    float errorX = x - filteredX;
    float errorY = y - filteredY;

    integratedErrorX += errorX;
    integratedErrorY += errorY;

    float speed = FSlowSpeed;
    if (fabs(errorX + errorY) > g_SlowZone) speed = FSpeed;

    if (fabs(errorX) > FDeadZone){ filteredX += speed * errorX; }
    if (fabs(errorY) > FDeadZone){ filteredY += speed * errorY; }

    if (abs(integratedErrorX) > FIntegrationDeadZone){ filteredX += integratingSpeed * integratedErrorX; integratedErrorX = 0; }
    if (abs(integratedErrorY) > FIntegrationDeadZone){ filteredY += integratingSpeed * integratedErrorY; integratedErrorY = 0; }

    tuple<float, float> filteredData(filteredX, filteredY);
    lastdata = filteredData;
    return filteredData;
}
