#include "MouseIntegration.hpp"

#include "NoiseCancellation.hpp"

#include <QtGlobal>

#ifdef Q_OS_LINUX
#include <X11/extensions/XTest.h>
#include <X11/extensions/Xrandr.h>
#endif

#ifdef Q_OS_WINDOWS
#include <windows.h>
#include <winuser.h>
#endif

#ifdef Q_OS_WIN
int CALLBACK MouseIntegration::EnumMonitors_CALLBACK(HMONITOR, HDC, LPRECT lPRect, LPARAM)
{
    Monitors.insert(Monitors.end(), *lPRect);
    ScreenWidth = lPRect->right - lPRect->left;
    ScreenHeight = lPRect->bottom - lPRect->top;
    return FALSE; // We currently only support the first one...
}
#endif

MouseIntegration::MouseIntegration()
{
    NoiseCancellation::init();
    // TODO: Multiple Display supports.
#ifdef Q_OS_LINUX
    X11_Display = XOpenDisplay(None);
    XRRScreenResources *screens = XRRGetScreenResources(X11_Display, XDefaultRootWindow(X11_Display));
    XRRCrtcInfo *info = XRRGetCrtcInfo(X11_Display, screens, screens->crtcs[0]);
    ScreenWidth = static_cast<int>(info->width);
    ScreenHeight = static_cast<int>(info->height);
    XRRFreeCrtcInfo(info);
    XRRFreeScreenResources(screens);
#elif Q_OS_WIN
    EnumDisplayMonitors(nullptr, nullptr, EnumMonitors_CALLBACK, NULL);
#endif
}

void MouseIntegration::SetWorkingMode(TobiiMouseWorkingMode mode)
{
    WorkingMode = mode;
}

void MouseIntegration::OnGaze(float x, float y)
{
    const auto &[XPos, YPos] = NoiseCancellation::CancelNoise((float) ScreenWidth * x, (float) ScreenHeight * y);
    switch (WorkingMode)
    {
        case MouseMode_MOVE_ABSOLUTE: MoveMouseTo(XPos, YPos); break;
        case MouseMode_MOVE_RELATIVE: MoveMouseOffset(XPos, YPos); break;
        case MouseMode_MOVE_BY_SECTIONS: MoveMouseByScreenSection(XPos, YPos); break;
    }
    std::tie(LastXPos, LastYPos) = std::tuple{ XPos, YPos };
}

void MouseIntegration::SetUseNewMouseMoveFunction(bool useNew)
{
    UseNewMouseMoveFunction = useNew;
}

void MouseIntegration::SetVThreashould(double Vth)
{
    VerticalThreshold = Vth;
}

void MouseIntegration::SetHThreashould(double Hth)
{
    HorizontalThreshold = Hth;
}

void MouseIntegration::SetMouseScaleFactor(double R)
{
    ScaleFactor = R;
}

void MouseIntegration::MoveMouseTo(int x, int y)
{
    x += DeltaX;
    y += DeltaY;

    emit OnAbsoluteMousePositionUpdated(x, y);

    if (doNotUpdateMouse)
        return;

#ifdef Q_OS_LINUX
    XTestFakeMotionEvent(X11_Display, 0, x, y, 0);
    XFlush(X11_Display);
#elif Q_OS_WIN
    SetCursorPos(x, y);
#endif
}

void MouseIntegration::MoveMouseOffset(int x, int y)
{
    if (doNotUpdateMouse)
        return;

#ifdef Q_OS_LINUX
    XTestFakeRelativeMotionEvent(X11_Display, x - LastXPos, y - LastYPos, 0);
    XFlush(X11_Display);
#elif Q_OS_WIN
    if (UseNewMouseMoveFunction)
    {
        INPUT ip;
        ip.type = INPUT_MOUSE;
        ip.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
        ip.mi.dx = (0xFFFF / ScreenWidth) * x;
        ip.mi.dy = (0xFFFF / ScreenHeight) * y;
        ip.mi.time = 0;
        ip.mi.mouseData = 0;
        SendInput(1, &ip, sizeof(ip));
    }
    else
    {
        mouse_event(MOUSEEVENTF_MOVE, x - LastX, y - LastY, 0, NULL);
    }
#endif
}

void MouseIntegration::MoveMouseByScreenSection(int x, int y)
{
    if (doNotUpdateMouse)
        return;

    const auto screenCenterX = static_cast<int>(ScreenWidth / 2);
    const auto screenCenterY = static_cast<int>(ScreenHeight / 2);
    const auto horizontalNoDetectRange = ((ScreenWidth - screenCenterX) * HorizontalThreshold);
    const auto verticalNoDetectRange = ((ScreenHeight - screenCenterY) * VerticalThreshold);
    const auto noDetectRect_Left = screenCenterX - horizontalNoDetectRange;
    const auto noDetectRect_Top = screenCenterY - verticalNoDetectRange;
    const auto noDetectRect_Right = screenCenterX + horizontalNoDetectRange;
    const auto noDetectRect_Bottom = screenCenterY + verticalNoDetectRange;

    if ((noDetectRect_Left < x && x < noDetectRect_Right) && (noDetectRect_Top < y && y < noDetectRect_Bottom))
        return;

    const bool isGazeOnLeft = x < screenCenterX;
    const bool isGazeOnTop = y < screenCenterY;
    const auto gazeRatioHorizontal = isGazeOnLeft ? fabs(noDetectRect_Left - x) / screenCenterX : fabs(x - noDetectRect_Right) / (screenCenterX);
    const auto gazeRatioVerticle = isGazeOnTop ? fabs(noDetectRect_Top - y) / screenCenterY : fabs(y - noDetectRect_Bottom) / (screenCenterY);
    const auto HSpeed = int(InitialSpeed + pow(gazeRatioHorizontal, 2) * ScaleFactor);
    const auto VSpeed = int(InitialSpeed + pow(gazeRatioVerticle, 2) * ScaleFactor);
#ifdef Q_OS_WIN
    const auto HSpeedl = static_cast<unsigned long>(HSpeed);
    const auto VSpeedl = static_cast<unsigned long>(VSpeed);
    mouse_event(MOUSEEVENTF_MOVE, isGazeOnLeft ? -HSpeedl : HSpeedl, isGazeOnTop ? -VSpeedl : VSpeedl, 0, NULL);
#else
    XTestFakeRelativeMotionEvent(X11_Display, isGazeOnLeft ? -HSpeed : HSpeed, isGazeOnTop ? -VSpeed : VSpeed, 0);
    XFlush(X11_Display);
#endif
}

void MouseIntegration::SetDeltaX(int x)
{
    DeltaX = x;
}

void MouseIntegration::SetDeltaY(int y)
{
    DeltaY = y;
}

void MouseIntegration::SetDoNotUpdateMouse(bool flag)
{
    doNotUpdateMouse = flag;
}
