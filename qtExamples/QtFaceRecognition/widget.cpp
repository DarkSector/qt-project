#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QCamera>
#include <QImageEncoderSettings>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    // Always start with 0
    ui->tabWidget->setCurrentIndex(0);

    // Initialize camera
    pCamera = new QCamera(this);
    pCameraViewfinder = new QCameraViewfinder(this);
    pCameraImageCapture = new QCameraImageCapture(pCamera, this);

    // Add layout
    pLayout = new QVBoxLayout;

    // Add link viewfinder with camera
    pCamera->setViewfinder(pCameraViewfinder);

    // And add the viewfinder widget to the layout
    pLayout->addWidget(pCameraViewfinder);

    // add layout to scroll area on the uI
    ui->scrollArea->setLayout(pLayout);

    setEncoding();
    pCamera->start();

    connect(pCameraImageCapture, SIGNAL(imageSaved(int,QString)), this, SLOT(onImageSaved(int ,QString)));

}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_tabWidget_currentChanged(int index)
{
    qDebug() << QString("Current tab index is %1").arg(index);
    switch(index){
        case 0:
            qDebug("Initializing camera");
            setEncoding();
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

void Widget::on_applicationClose_clicked()
{
    close();
}

void Widget::onImageSaved(int id, const QString &fileName){
    QString name = QString("Image saved at\n\n%1\n\nWould you like to use this? Press Yes to proceed or No to go back").arg(fileName);

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Image saved!", name, QMessageBox::Yes | QMessageBox::No);

    if(reply == QMessageBox::Yes){
        qDebug() << "Go to preview";
    }
}

void Widget::on_takePicture_clicked()
{

    QString destinationPath = ui->pathField->text();


    pCamera->searchAndLock();
    pCamera->unlock();

    if (destinationPath.size() > 0){
        pCameraImageCapture->capture(destinationPath);
    }
    else{
        pCameraImageCapture->capture();
    }
}


void Widget::setEncoding(){
    pCameraImageCapture->setCaptureDestination(QCameraImageCapture::CaptureToFile);
    QImageEncoderSettings imageEncoderSettings;
    imageEncoderSettings.setCodec("image/png");
    imageEncoderSettings.setResolution(720, 480);
    pCameraImageCapture->setEncodingSettings(imageEncoderSettings);
    pCamera->setCaptureMode(QCamera::CaptureStillImage);
}

void Widget::on_browseButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 "/home",
                                                 QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->pathField->setText(dir);
}
