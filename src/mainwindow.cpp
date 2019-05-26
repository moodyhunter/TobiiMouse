#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tobii_interactive.h"

#include <QMessageBox>
#include <QThread>
#include <list>

void MainWindow::reload_tobii_device_list()
{
    tobii->reload_devices();
    auto devices = tobii->devices;

    QStringList* deviceStringList = new QStringList();
    for (size_t i = 0; i < devices.size(); i++) {
        deviceStringList->append(QString::fromStdString(devices[i]));
    }
    ui->tobiiDevicesList->clear();
    ui->tobiiDevicesList->addItems(*deviceStringList);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    tobii(new TobiiInteractive())
{
    ui->setupUi(this);
    reload_tobii_device_list();
}

MainWindow::~MainWindow()
{
    delete tobii;
    delete ui;
}

void MainWindow::StartReadGaze()
{
    //QThread* thread = QThread::create(QFunctionPointer(process));
    //on_GetAllDevices_clicked();
    //thread->start();
}
void MainWindow::on_reloadListButton_clicked()
{
    reload_tobii_device_list();
}

void MainWindow::on_useSelectedDeviceButton_clicked()
{
    if(ui->tobiiDevicesList->selectedItems().count() > 0){
        QString currentSelected = ui->tobiiDevicesList->selectedItems().first()->text();
        tobii->start_subscribe_gaze(currentSelected.toStdString().c_str());
    }
}
