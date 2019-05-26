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
    void reload_tobii_device_list();

private slots:
    void on_reloadListButton_clicked();

    void on_useSelectedDeviceButton_clicked();

private:
    Ui::MainWindow *ui;
    TobiiInteractive *tobii;
};

#endif // MAINWINDOW_H
