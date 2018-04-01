#include "helloworldwidget.h"
#include "ui_helloworldwidget.h"

HelloWorldWidget::HelloWorldWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HelloWorldWidget)
{
    ui->setupUi(this);
}

HelloWorldWidget::~HelloWorldWidget()
{
    delete ui;
}
