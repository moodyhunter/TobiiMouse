#ifndef MOUSE_INTEGRATION_H
#define MOUSE_INTEGRATION_H

#include <tuple>
#include <QtCore>
#include "noise_cancellation.h"

#ifdef __linux__
#include <X11/X.h>
#include "X11/Xlib.h"
#include <X11/extensions/XTest.h>
#include <X11/extensions/Xrandr.h>
#elif _WIN32
#include <windows.h>
#include <winuser.h>
#else
#error "Unknown Operating System..."
#endif

using namespace std;

enum MOUSEWORKINGMODE_E {
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
    WINBOOL CALLBACK EnumMonitors_CALLBACK(HMONITOR a, HDC b, LPRECT c, LPARAM d);
#endif

};

#endif // MOUSE_INTEGRATION_H
