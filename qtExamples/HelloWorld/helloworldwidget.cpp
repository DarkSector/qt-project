#include "helloworldwidget.h"
#include "ui_helloworldwidget.h"
#include <QPushButton>
#include <QDebug>
#include <QRect>
#include <QLabel>

HelloWorldWidget::HelloWorldWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HelloWorldWidget)
{
    // constructor
    ui->setupUi(this);
    pushButton1 = new QPushButton("Press Me", this);
    pushButton1->setGeometry(QRect(100, 100, 100, 100));
    label = new QLabel("label1", this);
    label->hide();
    // add a slot
    connect(pushButton1, SIGNAL (clicked()), this, SLOT (on_pushButton_clicked()));
}

HelloWorldWidget::~HelloWorldWidget()
// destructor
{
    delete ui;
}

void HelloWorldWidget::on_pushButton_clicked()
{
    label->setText("Hello World");
    label->show();

}
