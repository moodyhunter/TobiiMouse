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
    void StartReadGaze();

private slots:
    void on_GetAllDevices_clicked();

private:
    Ui::MainWindow *ui;
    TobiiInteractive *tobii;
};

#endif // MAINWINDOW_H
