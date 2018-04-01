#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    launchButton = new QPushButton("Launch Python Script", this);
    launchButton->setGeometry(QRect(100, 100, 200, 100));
    connect(launchButton, SIGNAL(clicked()), this, SLOT(on_launchButton_press()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_launchButton_press(){
    qDebug("Button pressed");

}
