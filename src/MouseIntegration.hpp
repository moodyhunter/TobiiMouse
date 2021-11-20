#pragma once

#include <QtCore>
#include <tuple>

enum TobiiMouseWorkingMode
{
    MouseMode_MOVE_ABSOLUTE,
    MouseMode_MOVE_RELATIVE,
    MouseMode_MOVE_BY_SECTIONS
};

namespace MouseIntegration
{
    void init();
    void OnGaze(float x, float y);

    void MoveMouseTo(int x, int y);
    void MoveMouseOffset(int x, int y);
    void MoveMouseByScreenSection(int x, int y);

    void SetWorkingMode(TobiiMouseWorkingMode mode);
    void SetUseNewMouseMoveFunction(bool useNew);
    void SetVThreashould(double Vth);
    void SetHThreashould(double Hth);
    void SetMouseScaleFactor(double R);
}; // namespace MouseIntegration
