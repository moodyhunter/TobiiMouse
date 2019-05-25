#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tobii_interactive.h"

#include <QMessageBox>
using namespace Devices;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    tobii(new Devices::TobiiInteractive)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startButton_clicked()
{
    //tobii->testmain();
    QMessageBox *m = new QMessageBox();
    m->setText("Show text!!!!xxxxxxxxxxxxxxxxxxxxx");
    m->setWindowTitle("MyTitle");
    m->show();
    printf("Clicked!!!1");
}
