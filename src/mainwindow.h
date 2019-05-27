#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tobii_interactive.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void startReadGaze();
    void reloadTobiiDeviceList();
    void OnGazePositionReceived(string x, string y);

//public slots:
//    void OnGazePositionReceived( tobii_gaze_point_t const* gaze_point );

private slots:
    void on_reloadListButton_clicked();

    void on_useSelectedDeviceButton_clicked();
private:
    Ui::MainWindow *ui;
    QThread* gazeThread;
};


#endif // MAINWINDOW_H
