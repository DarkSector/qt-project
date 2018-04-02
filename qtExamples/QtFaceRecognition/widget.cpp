#include "widget.h"
#include "ui_widget.h"
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
    pCameraImageCapture = new QCameraImageCapture(this);
    pVBoxLayout = new QVBoxLayout(this);

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
