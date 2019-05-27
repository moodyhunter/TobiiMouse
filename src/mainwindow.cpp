#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tobii_interactive.h"

#include <QMessageBox>
#include <QThread>
#include <list>
#include <iostream>


using namespace std;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)

{
    ui->setupUi(this);
    TobiiInteractive::init();
    reloadTobiiDeviceList();
    gazeThread = new QThread();

    //QObject::connect(&tobii, &TobiiInteractive::gazePositionChanged, this, &MainWindow::OnGazePositionReceived);
}

void MainWindow::OnGazePositionReceived(string x, string y){
        ui->gazePositionX->display(QString::fromStdString(x));
        ui->gazePositionY->display(QString::fromStdString(y));
        cout << "x: " << x << " ----- y: " << y << endl;
}

void MainWindow::reloadTobiiDeviceList()
{
    auto devices = TobiiInteractive::reload_devices();
    ui->tobiiDevicesList->clear();
    for (size_t i = 0; i < devices.size(); i++) {
        ui->tobiiDevicesList->addItem(QString::fromStdString(devices[i]));
    }
}
MainWindow::~MainWindow()
{
    delete ui;
}

//void MainWindow::gaze_point_callback( tobii_gaze_point_t const* gaze_point, void* user_data )
//{
//    assert(user_data == nullptr);
//    if( gaze_point->validity == TOBII_VALIDITY_VALID )
//        cout << "Gaze point: " << gaze_point->position_xy[ 0 ] << " - " << gaze_point->position_xy[ 1 ] << endl;
//}



void MainWindow::startReadGaze()
{
    //QThread* thread = QThread::create(QFunctionPointer(process));
    //on_GetAllDevices_clicked();
    //thread->start();
}
void MainWindow::on_reloadListButton_clicked()
{
    reloadTobiiDeviceList();
}

void MainWindow::on_useSelectedDeviceButton_clicked()
{
    if(ui->tobiiDevicesList->selectedItems().count() > 0){
        QString currentSelected = ui->tobiiDevicesList->selectedItems().first()->text();
        ui->currentDeviceLabel->setText(currentSelected);
        TobiiInteractive::start_subscribe_gaze(currentSelected.toStdString().c_str(), this);
    }
}
