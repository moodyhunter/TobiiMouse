/*
 * This is an example that demonstrates how to connect to the EyeX Engine and subscribe to the lightly filtered gaze data stream.
 *
 * Copyright 2013-2014 Tobii Technology AB. All rights reserved.
 * Copyright 2019 Leroy.H.Y (lhy20010403@hotmail.com).
 */

#include "NoiseCancellation.hpp"

namespace NoiseCancellation
{
    static std::tuple<float, float> lastdata;

    static constexpr const float FDeadZone = 8.0;    // pixels
    static constexpr const float g_SlowZone = 40;    // pixels
    static constexpr const float FSpeed = 0.5f;      // how fast to move when far away
    static constexpr const float FSlowSpeed = 0.04f; // now fast to move when close

    static constexpr const float FIntegrationDeadZone = 1.0; // pixels
    static constexpr const float integratingSpeed = 0.2f;

    static float lastRawX = 0;
    static float lastRawY = 0;

    static float filteredX = 0;
    static float filteredY = 0;

    static float integratedErrorX = 0;
    static float integratedErrorY = 0;

    // static float filteredErrorX = 0; // this will be the distance between the gaze data and the current mouse position
    // static float filteredErrorY = 0; // this will be the distance between the gaze data and the current mouse position
} // namespace NoiseCancellation

void NoiseCancellation::init()
{
}

std::tuple<float, float> NoiseCancellation::CancelNoise(float x, float y)
{
    if (pow(fabs(lastRawX - x), 2) + pow(fabs(lastRawY - y), 2) > 320)
    { // For some huge errors, such as the blinking eyes.
        lastRawX = x;
        lastRawY = y;
        return lastdata; // reject noise
    }

    float errorX = x - filteredX;
    float errorY = y - filteredY;
    integratedErrorX += errorX;
    integratedErrorY += errorY;
    float speed = FSlowSpeed;

    if (std::fabs(errorX + errorY) > g_SlowZone)
        speed = FSpeed;

    if (std::fabs(errorX) > FDeadZone)
    {
        filteredX += speed * errorX;
    }

    if (std::fabs(errorY) > FDeadZone)
    {
        filteredY += speed * errorY;
    }

    if (std::abs(integratedErrorX) > FIntegrationDeadZone)
    {
        filteredX += integratingSpeed * integratedErrorX;
        integratedErrorX = 0;
    }

    if (std::abs(integratedErrorY) > FIntegrationDeadZone)
    {
        filteredY += integratingSpeed * integratedErrorY;
        integratedErrorY = 0;
    }

    std::tuple<float, float> filteredData(filteredX, filteredY);
    lastdata = filteredData;
    return filteredData;
}
