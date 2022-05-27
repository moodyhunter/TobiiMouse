#pragma once

#include <QMainWindow>

class GazeTracker;
class MouseIntegration;

namespace TobiiMouse
{
    class TobiiDevice;
    class TobiiAPI;
} // namespace TobiiMouse

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void reloadTobiiDeviceList();
    void OnGazePositionUIUpdate(float x, float y);

  private slots:
    void on_reloadListButton_clicked();
    void on_useSelectedDeviceButton_clicked();
    void on_actionQuit_triggered();
    void on_absoluteButton_clicked(bool checked);
    void on_relativeButton_clicked(bool checked);
    void on_radioButton_clicked(bool checked);
    void on_useNewMouseEvent_stateChanged(int arg1);
    void on_doubleSpinBox_3_valueChanged(double arg1);
    void on_doubleSpinBox_2_valueChanged(double arg1);
    void on_doubleSpinBox_valueChanged(double arg1);
    void on_deltaXSB_valueChanged(int arg1);
    void on_deltaYSB_valueChanged(int arg1);

    void on_showGazeCB_stateChanged(int arg1);

  public slots:
    void OnMouseMoved(int x, int y);

  private:
    Ui::MainWindow *ui;
    MouseIntegration *mouseIntegration = nullptr;
    TobiiMouse::TobiiAPI *tobiiInteraction = nullptr;
    TobiiMouse::TobiiDevice *currentDevice = nullptr;
    GazeTracker *tracker;
};
