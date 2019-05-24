#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tobii_interactive.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startButton_clicked()
{
    QMessageBox *m = new QMessageBox();
    m->setText("Show text!!!!xxxxxxxxxxxxxxxxxxxxx");
    m->setWindowTitle("MyTitle");
    m->show();
    printf("Clicked!!!1");
}
