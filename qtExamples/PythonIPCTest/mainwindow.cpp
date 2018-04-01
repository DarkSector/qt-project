#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    launchButton = new QPushButton("Launch Python Script", this);
    launchButton->setGeometry(QRect(100, 100, 200, 100));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_button_press(){

}
