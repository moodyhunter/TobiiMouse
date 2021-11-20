#pragma once

#include <QtCore>
#include <tuple>

#ifdef _WIN32
#include <windows.h>
#include <winuser.h>
#endif

// using namespace std;

enum MOUSEWORKINGMODE_E
{
    TOBII_MOUSE_MODE_MOVE_ABSOLUTE,
    TOBII_MOUSE_MODE_MOVE_RELATIVE,
    TOBII_MOUSE_MODE_MOVE_BY_SECTIONS
};

namespace MouseIntegration
{
    void init();
    void OnGaze(float x, float y);
    //
    void MoveMouseTo(int x, int y);
    void MoveMouseOffset(int x, int y);
    void MoveMouseByScreenSection(int x, int y);
    //
    void SetWorkingMode(MOUSEWORKINGMODE_E mode);
    void SetUseNewMouseMoveFunction(bool useNew);
    void SetVThreashould(double Vth);
    void SetHThreashould(double Hth);
    void SetMouseScaleFactor(double R);
    //
#ifdef _WIN32
    int CALLBACK EnumMonitors_CALLBACK(HMONITOR a, HDC b, LPRECT c, LPARAM d);
#endif

}; // namespace MouseIntegration
