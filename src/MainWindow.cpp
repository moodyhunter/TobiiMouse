#include "MainWindow.hpp"

#include "GazeTracker.hpp"
#include "MouseIntegration.hpp"
#include "TobiiAPI.hpp"
#include "TobiiDevice.hpp"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), mouseIntegration(new MouseIntegration), tracker(new GazeTracker)
{
    ui->setupUi(this);
    tobiiInteraction = new TobiiMouse::TobiiAPI(this);
    tracker->show();

    connect(mouseIntegration, &MouseIntegration::OnAbsoluteMousePositionUpdated, this, &MainWindow::OnMouseMoved);
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
    tracker->close();
    delete tracker;
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
                    if (x <= 0 && y <= 0)
                        return;
                    OnGazePositionUIUpdate(x, y);
                    mouseIntegration->OnGaze(x, y);
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
        mouseIntegration->SetWorkingMode(MouseMode_MOVE_ABSOLUTE);
}

void MainWindow::on_relativeButton_clicked(bool checked)
{
    if (checked)
        mouseIntegration->SetWorkingMode(MouseMode_MOVE_RELATIVE);
}

void MainWindow::on_radioButton_clicked(bool checked)
{
    if (checked)
        mouseIntegration->SetWorkingMode(MouseMode_MOVE_BY_SECTIONS);
}

void MainWindow::on_useNewMouseEvent_stateChanged(int arg1)
{
    mouseIntegration->SetUseNewMouseMoveFunction(arg1 == Qt::Checked);
}

void MainWindow::on_doubleSpinBox_3_valueChanged(double arg1)
{
    // V
    mouseIntegration->SetVThreashould(arg1);
}

void MainWindow::on_doubleSpinBox_2_valueChanged(double arg1)
{
    // H
    mouseIntegration->SetHThreashould(arg1);
}

void MainWindow::on_doubleSpinBox_valueChanged(double arg1)
{
    // R
    mouseIntegration->SetMouseScaleFactor(arg1);
}

void MainWindow::on_deltaXSB_valueChanged(int arg1)
{
    mouseIntegration->SetDeltaX(arg1);
}

void MainWindow::on_deltaYSB_valueChanged(int arg1)
{
    mouseIntegration->SetDeltaY(arg1);
}

void MainWindow::OnMouseMoved(int x, int y)
{
    tracker->MoveTo(x, y);
}

void MainWindow::on_showGazeCB_stateChanged(int arg1)
{
    mouseIntegration->SetDoNotUpdateMouse(ui->showGazeCB->isChecked());
}
