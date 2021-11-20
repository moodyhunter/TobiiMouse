#pragma once

#include <cmath>
#include <tuple>

namespace NoiseCancellation
{
    void init();
    std::tuple<float, float> CancelNoise(float x, float y);
}; // namespace NoiseCancellation
