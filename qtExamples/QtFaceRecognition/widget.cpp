#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QVBoxLayout>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    pCamera = new QCamera(this);
    pCameraViewfinder = new QCameraViewfinder(this);
    pCameraImageCapture = new QCameraImageCapture(pCamera, this);
    pLayout = new QVBoxLayout;
    pCamera->setViewfinder(pCameraViewfinder);
    pLayout->addWidget(pCameraViewfinder);
    ui->scrollArea->setLayout(pLayout);

}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    // Terminate application
    close();
}

void Widget::on_pushButton_2_clicked()
{
    // Take picture here

}

void Widget::on_tabWidget_currentChanged(int index)
{
    qDebug() << QString("Current tab index is %1").arg(index);
    switch(index){
        case 0:
            qDebug("Initializing camera");
            pCamera->start();
            break;
//        case 1:
//            qDebug("Gallery");
//            pCamera->stop();
//            break;
//        case 2:
//            qDebug("Analysis");
//            pCamera->stop();
//            break;
        default:
            qDebug("Stopping camera");
            pCamera->stop();
            break;
    }
}

void Widget::on_pushButton_5_clicked()
{

}

void Widget::on_pushButton_6_clicked()
{

}
