#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tobii_interactive.h"
#include "mouse_integration.h"

#include <QMessageBox>
#include <QThread>
#include <list>
#include <iostream>


using namespace std;

MainWindow *MainWindow::instance = nullptr;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)

{
    ui->setupUi(this);
    TobiiInteractive::Initialise();
    reloadTobiiDeviceList();
    MainWindow::instance = this;
}

void MainWindow::OnGazePositionUIUpdate(float x, float y)
{
    ui->gazePositionX->display(QString::number(x));
    ui->gazePositionY->display(QString::number(y));
}

void MainWindow::reloadTobiiDeviceList()
{
    auto devices = TobiiInteractive::ReloadDevices();
    ui->tobiiDevicesList->clear();
    ui->tobiiDevicesList->addItems(devices);

    if (ui->tobiiDevicesList->count() > 0) {
        ui->tobiiDevicesList->setCurrentItem(ui->tobiiDevicesList->item(0));
    }
}
MainWindow::~MainWindow()
{
    TobiiInteractive::StopSubscribeGaze();
    delete ui;
}

void MainWindow::on_reloadListButton_clicked()
{
    reloadTobiiDeviceList();
}

void MainWindow::on_useSelectedDeviceButton_clicked()
{
    if (!ui->tobiiDevicesList->selectedItems().isEmpty()) {
        QString currentSelected = ui->tobiiDevicesList->selectedItems().first()->text();
        ui->currentDeviceLabel->setText(currentSelected);
        TobiiInteractive::StartSubscribeGaze(currentSelected);
    }
}

void MainWindow::on_actionQuit_triggered()
{
    TobiiInteractive::StopSubscribeGaze();
    QApplication::quit();
}

void MainWindow::on_absoluteButton_clicked(bool checked)
{
    if (checked) MouseIntegration::SetWorkingMode(TOBII_MOUSE_MODE_MOVE_ABSOLUTE);
}

void MainWindow::on_relativeButton_clicked(bool checked)
{
    if (checked) MouseIntegration::SetWorkingMode(TOBII_MOUSE_MODE_MOVE_RELATIVE);
}

void MainWindow::on_radioButton_clicked(bool checked)
{
    if (checked) MouseIntegration::SetWorkingMode(TOBII_MOUSE_MODE_MOVE_BY_SECTIONS);
}

void MainWindow::on_useNewMouseEvent_stateChanged(int arg1)
{
    MouseIntegration::SetUseNewMouseMoveFunction(arg1 == Qt::Checked);
}

void MainWindow::on_doubleSpinBox_3_valueChanged(double arg1)
{
    //V
    MouseIntegration::SetVThreashould(arg1);
}

void MainWindow::on_doubleSpinBox_2_valueChanged(double arg1)
{
    //H
    MouseIntegration::SetHThreashould(arg1);
}

void MainWindow::on_doubleSpinBox_valueChanged(double arg1)
{
    //R
    MouseIntegration::SetMouseScaleFactor(arg1);
}
