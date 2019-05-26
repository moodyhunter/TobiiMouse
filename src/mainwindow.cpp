#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tobii_interactive.h"

#include <QMessageBox>
#include <QThread>
#include <list>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    tobii(new TobiiInteractive())
{
    ui->setupUi(this);
    auto devices = tobii->devices;

    QStringList* deviceStringList = new QStringList();
    for (size_t i = 0; i < devices.size(); i++) {
        deviceStringList->append(QString::fromStdString(devices[i]));
    }

    ui->tobiiDevicesList->addItems(*deviceStringList);
}

MainWindow::~MainWindow()
{
    delete tobii;
    delete ui;
}

void MainWindow::StartReadGaze()
{
    //QThread* thread = QThread::create(QFunctionPointer(process));
    on_GetAllDevices_clicked();
    //thread->start();
}


void MainWindow::on_GetAllDevices_clicked()
{
    tobii->testmain();
}
