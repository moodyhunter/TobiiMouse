#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tobii_interactive.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        static MainWindow *instance;
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow();
        void reloadTobiiDeviceList();
        void OnGazePositionUIUpdate(float x, float y);

        //public slots:
        //    void OnGazePositionReceived( tobii_gaze_point_t const* gaze_point );

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

        void on_tobiiDevicesList_currentRowChanged(int currentRow);

      private:
        Ui::MainWindow *ui;
};


#endif // MAINWINDOW_H
