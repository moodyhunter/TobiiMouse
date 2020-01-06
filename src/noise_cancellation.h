#ifndef NOISE_CANCELLATION_H
#define NOISE_CANCELLATION_H
#include <tuple>
#include <cmath>
using namespace std;
namespace NoiseCancellation
{
    void init();
    tuple<float, float> CancelNoise(float x, float y);
};

#endif // NOISE_CANCELLATION_H
