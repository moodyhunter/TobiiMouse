#pragma once

#include <QtCore>
#include <tuple>

typedef struct _XDisplay Display;

enum TobiiMouseWorkingMode
{
    MouseMode_MOVE_ABSOLUTE,
    MouseMode_MOVE_RELATIVE,
    MouseMode_MOVE_BY_SECTIONS
};

class MouseIntegration : public QObject
{
    Q_OBJECT

  public:
    explicit MouseIntegration();

    void OnGaze(float x, float y);

    void MoveMouseTo(int x, int y);
    void MoveMouseOffset(int x, int y);
    void MoveMouseByScreenSection(int x, int y);

    void SetWorkingMode(TobiiMouseWorkingMode mode);
    void SetUseNewMouseMoveFunction(bool useNew);
    void SetVThreashould(double Vth);
    void SetHThreashould(double Hth);
    void SetMouseScaleFactor(double R);

    void SetDeltaX(int x);
    void SetDeltaY(int y);

    void SetDoNotUpdateMouse(bool);

  signals:
    void OnAbsoluteMousePositionUpdated(int x, int y);

  private:
    TobiiMouseWorkingMode WorkingMode;
    int ScreenHeight;
    int ScreenWidth;
    int LastXPos;
    int LastYPos;

    bool doNotUpdateMouse = false;

    // For Move by screen sectors.
    int InitialSpeed = 0;    // pixels
    double ScaleFactor = 16; // factor of square when gaze is near the margin.
    double HorizontalThreshold = 0.15;
    double VerticalThreshold = 0.15;

    bool UseNewMouseMoveFunction = false;

    int DeltaX;
    int DeltaY;

#ifdef Q_OS_LINUX
    Display *X11_Display;
#elif Q_OS_WIN
    std::vector<RECT> Monitors;
#endif
}; // namespace MouseIntegration
