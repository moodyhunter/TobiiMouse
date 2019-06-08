#ifndef MOUSE_INTEGRATION_H
#define MOUSE_INTEGRATION_H

#include <tuple>
#include "tobiimouse.h"
#include "noise_cancellation.h"

#ifdef __linux__
#include <X11/X.h>
#include "X11/Xlib.h"
#include <X11/extensions/XTest.h>
#include <X11/extensions/Xrandr.h>
#elif _WIN32
#include <windows.h>
#include <winuser.h>
// windows code goes here
#else
#error "Unknown Operating System..."
#endif

using namespace std;

enum MOUSEWORKINGMODE_E{
    TOBII_MOUSE_MODE_MOVE_ABSOLUTE,
    TOBII_MOUSE_MODE_MOVE_RELATIVE,
    TOBII_MOUSE_MODE_MOVE_BY_SECTIONS
};

namespace MouseIntegration
{
void SetWorkingMode(MOUSEWORKINGMODE_E mode);
void init();
void MoveMouseTo(int x, int y);
void MoveMouseOffset(int x, int y);
tuple<int, int> ProcessGazePosition(float x, float y);
void OnGaze(float x, float y);
void MoveMouseByScreenSection(int x, int y);

#ifdef _WIN32
WINBOOL CALLBACK EnumMonitors_CALLBACK(HMONITOR a,HDC b,LPRECT c,LPARAM d);
#endif

};

#endif // MOUSE_INTEGRATION_H
