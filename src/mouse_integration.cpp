#include "mouse_integration.h"
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
#else
#error "Unknown Operating System..."
#endif
}

#ifdef _WIN32
WINBOOL CALLBACK MouseIntegration::EnumMonitors_CALLBACK(HMONITOR hmonitor, HDC hdc,LPRECT lPRect, LPARAM _param){
    UNUSED(hmonitor);
    UNUSED(hdc);
    UNUSED(_param);
    Monitors.insert(Monitors.end(), *lPRect);
    ScreenWidth = lPRect->right - lPRect->left;
    ScreenHeight = lPRect->bottom - lPRect->top;
    return FALSE; // We currently only support the first one...
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
    // windows code goes here
#endif
}

void MouseIntegration::MoveMouseOffset(int x, int y)
{
#ifdef __linux__
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
    case TOBII_MOUSE_MODE_MOVE_BY_SECTIONS:
        break;
    }
    LastX = posiX;
    LastY = posiY;
}
