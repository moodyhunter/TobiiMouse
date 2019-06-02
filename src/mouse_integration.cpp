#include "mouse_integration.h"
#include <iostream>
#include <vector>

namespace MouseIntegration
{
static MOUSEWORKINGMODE_E WorkingMode;
static int ScreenHeight;
static int ScreenWidth;

static int LastX;
static int LastY;

#ifdef __linux__
static Display* _display;
static Window _root_window;
#elif _WIN32
static vector<RECT> Monitors;
#endif
}

#ifdef _WIN32
namespace MouseIntegration {
static int InitialSpeed = 1; // pixels
static double SquareFactor = 15; // factor of square when gaze is near the margin.
static double HorizontalThrehold = 0.13;
static double VerticalThrehold = 0.13;
}
WINBOOL CALLBACK MouseIntegration::EnumMonitors_CALLBACK(HMONITOR hmonitor, HDC hdc,LPRECT lPRect, LPARAM _param){
    UNUSED(hmonitor);
    UNUSED(hdc);
    UNUSED(_param);
    Monitors.insert(Monitors.end(), *lPRect);
    ScreenWidth = lPRect->right - lPRect->left;
    ScreenHeight = lPRect->bottom - lPRect->top;
    return FALSE; // We currently only support the first one...
}
void MouseIntegration::MoveMouseByScreenSection(int x, int y)
{
    auto screenCenterX = static_cast<int>(ScreenWidth / 2);
    auto screenCenterY = static_cast<int>(ScreenHeight / 2);

    auto horizontalNoDetectRange = ((ScreenWidth - screenCenterX) * HorizontalThrehold);
    auto verticalNoDetectRange = ((ScreenHeight - screenCenterY) * VerticalThrehold);

    auto noDetectRect_Left = screenCenterX - horizontalNoDetectRange;
    auto noDetectRect_Top = screenCenterY - verticalNoDetectRange;
    auto noDetectRect_Right = screenCenterX + horizontalNoDetectRange;
    auto noDetectRect_Bottom = screenCenterY + verticalNoDetectRange;

    if((noDetectRect_Left < x && x < noDetectRect_Right) && (noDetectRect_Top < y && y < noDetectRect_Bottom)) return;

    bool isGazeOnLeft = x < screenCenterX;
    bool isGazeOnTop = y < screenCenterY;

    auto gazeRatioHorizontal = isGazeOnLeft
                ? fabs(noDetectRect_Left - x) / screenCenterX
                : fabs(x - noDetectRect_Right) / (screenCenterX);

    auto gazeRatioVerticle = isGazeOnTop
                ? fabs(noDetectRect_Top - y) / screenCenterY
                : fabs(y - noDetectRect_Bottom) / (screenCenterY);

    auto HSpeed = static_cast<unsigned long>(InitialSpeed + pow(gazeRatioHorizontal, 2) * SquareFactor);
    auto VSpeed = static_cast<unsigned long>(InitialSpeed + pow(gazeRatioVerticle, 2) * SquareFactor);
    mouse_event(MOUSEEVENTF_MOVE, isGazeOnLeft ? -HSpeed : HSpeed, isGazeOnTop ? -VSpeed : VSpeed, 0, NULL);

}
#endif
void MouseIntegration::init()
{
    NoiseCancellation::init();
    // TODO: Multiple Display supports.
#ifdef __linux__
    _display = XOpenDisplay(None);
    _root_window = XRootWindow(_display, 0);
    XRRScreenResources *screens = XRRGetScreenResources(_display, DefaultRootWindow(_display));
    XRRCrtcInfo *info = XRRGetCrtcInfo(_display, screens, screens->crtcs[0]);
    ScreenWidth = info->width;
    ScreenHeight = info->height;
    XRRFreeCrtcInfo(info);
    XRRFreeScreenResources(screens);
#elif _WIN32
    EnumDisplayMonitors(nullptr, nullptr, EnumMonitors_CALLBACK, NULL);
#endif
}


void MouseIntegration::MoveMouseTo(int x, int y)
{
#ifdef __linux__
    XSelectInput(_display, _root_window, KeyReleaseMask);
    XWarpPointer(_display, None, _root_window, 0, 0, 0, 0, x, y);
    XFlush(_display);
#elif _WIN32
    SetCursorPos(x, y);
#endif
}

void MouseIntegration::MoveMouseOffset(int x, int y)
{
#ifdef __linux__
    cout << "Not Implemented yet" << endl;
#elif _WIN32
    bool UseNewMouseMoveFunction = false;
    if(UseNewMouseMoveFunction)
    {
        INPUT ip;
        ip.type = INPUT_MOUSE;
        ip.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
        ip.mi.dx = ((x) * (0xFFFF / ScreenWidth));
        ip.mi.dy = ((y) * (0xFFFF / ScreenHeight));
        ip.mi.time = 0;
        ip.mi.mouseData = 0;
        SendInput(1, &ip, sizeof (ip));
    }
    else
    {
        mouse_event(MOUSEEVENTF_MOVE, x - LastX, y - LastY, 0, NULL);
    }

#endif
}

void MouseIntegration::SetWorkingMode(MOUSEWORKINGMODE_E mode)
{
    WorkingMode = mode;
}

tuple<int, int> MouseIntegration::ProcessGazePosition(float x, float y)
{
    float width = ScreenWidth;
    float height = ScreenHeight;

    auto realGazeX = width * x;
    auto realGazeY = height * y;

    auto filtered = NoiseCancellation::CancelNoise(realGazeX,realGazeY);
    //auto count = ScreenCount(_display); //Get total screen count.
    float gazeX_f = get<0>(filtered);
    float gazeY_f = get<1>(filtered);


    return tuple<int, int>(static_cast<int>(gazeX_f), static_cast<int>(gazeY_f));
}

void MouseIntegration::OnGaze(float x, float y)
{
    auto data = ProcessGazePosition(x,y);
    auto posiX = get<0>(data);
    auto posiY = get<1>(data);

    switch (WorkingMode) {
    case TOBII_MOUSE_MODE_ABSOLUTE:
        MoveMouseTo(posiX, posiY);
        break;
    case TOBII_MOUSE_MODE_MOVE_BY_POSITION:
        MoveMouseOffset(posiX, posiY);
        break;
#ifdef _WIN32
    case TOBII_MOUSE_MODE_MOVE_BY_SECTIONS:
        MoveMouseByScreenSection(posiX, posiY);
#endif
        break;
    }
    LastX = posiX;
    LastY = posiY;
}
