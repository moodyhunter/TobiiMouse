#include "MainWindow.hpp"

#include "MouseIntegration.hpp"
#include "TobiiAPI.hpp"
#include "TobiiDevice.hpp"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tobiiInteraction = new TobiiMouse::TobiiAPI(this);
    MouseIntegration::init();
    reloadTobiiDeviceList();
}

void MainWindow::OnGazePositionUIUpdate(float x, float y)
{
    ui->gazePositionX->display(QString::number(x));
    ui->gazePositionY->display(QString::number(y));
}

void MainWindow::reloadTobiiDeviceList()
{
    auto devices = tobiiInteraction->GetDevices();
    ui->tobiiDevicesList->clear();
    ui->tobiiDevicesList->addItems(devices);

    if (ui->tobiiDevicesList->count() > 0)
        ui->tobiiDevicesList->setCurrentItem(ui->tobiiDevicesList->item(0));
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_reloadListButton_clicked()
{
    reloadTobiiDeviceList();
}

void MainWindow::on_useSelectedDeviceButton_clicked()
{
    if (!ui->tobiiDevicesList->selectedItems().isEmpty())
    {
        QString currentSelected = ui->tobiiDevicesList->currentItem()->text();
        ui->currentDeviceLabel->setText(currentSelected);
        if (currentDevice)
            delete currentDevice;
        currentDevice = tobiiInteraction->OpenDevice(currentSelected);
        connect(currentDevice, &TobiiMouse::TobiiDevice::OnGazeDataReady, this,
                [this](float x, float y)
                {
                    OnGazePositionUIUpdate(x, y);
                    MouseIntegration::OnGaze(x, y);
                });
        currentDevice->SubscribeGazeData();
    }
}

void MainWindow::on_actionQuit_triggered()
{
    if (currentDevice)
    {
        currentDevice->UnsubscribeGazeData();
        disconnect(currentDevice);
    }
    QApplication::quit();
}

void MainWindow::on_absoluteButton_clicked(bool checked)
{
    if (checked)
        MouseIntegration::SetWorkingMode(TOBII_MOUSE_MODE_MOVE_ABSOLUTE);
}

void MainWindow::on_relativeButton_clicked(bool checked)
{
    if (checked)
        MouseIntegration::SetWorkingMode(TOBII_MOUSE_MODE_MOVE_RELATIVE);
}

void MainWindow::on_radioButton_clicked(bool checked)
{
    if (checked)
        MouseIntegration::SetWorkingMode(TOBII_MOUSE_MODE_MOVE_BY_SECTIONS);
}

void MainWindow::on_useNewMouseEvent_stateChanged(int arg1)
{
    MouseIntegration::SetUseNewMouseMoveFunction(arg1 == Qt::Checked);
}

void MainWindow::on_doubleSpinBox_3_valueChanged(double arg1)
{
    // V
    MouseIntegration::SetVThreashould(arg1);
}

void MainWindow::on_doubleSpinBox_2_valueChanged(double arg1)
{
    // H
    MouseIntegration::SetHThreashould(arg1);
}

void MainWindow::on_doubleSpinBox_valueChanged(double arg1)
{
    // R
    MouseIntegration::SetMouseScaleFactor(arg1);
}
