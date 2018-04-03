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
#include <QPixmap>
#include <QGraphicsView>
#include <QUrl>
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

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

    // Connect signal coming from image capture and map it with a private slot
    connect(pCameraImageCapture, SIGNAL(imageSaved(int,QString)), this, SLOT(onImageSaved(int ,QString)));

    // Graphics initialization
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

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
        case 1:
            qDebug("Gallery");
            pCamera->stop();
            break;
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

    // Question message content
    QString name = QString("Image saved at\n\n"
                           "%1\n\n"
                           "Would you like to use this? Press Yes to proceed or No to go back").arg(fileName);

    // Message box reply comes back
    QMessageBox::StandardButton reply = QMessageBox::question(this,
                                                              "Image saved!",
                                                              name,
                                                             QMessageBox::Yes | QMessageBox::No);

    // If yes, switch to preview
    if(reply == QMessageBox::Yes){
        qDebug() << "Go to preview";
        ui->tabWidget->setCurrentIndex(1);

        ui->statusLabel->setText(fileName);

        // new image
        image = new QGraphicsPixmapItem(QPixmap::fromImage(QImage(fileName)));

        // remove all items before you add another item
        scene->clear();

        // Add the new image item
        scene->addItem(image);
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

void Widget::on_loadImage_clicked()
{

    // Open filedialog at /home
    QString filename = QFileDialog::getOpenFileName(this, tr("Load Image"),
                                                    "/home",
                                                    "Images (*.png; *.jpeg; *.jpg)");
    // if nothing is selected, do nothing, just exit
    if (filename.isEmpty()){
        return;
    }

    // set the status label to the item name
    ui->statusLabel->setText(filename);

    // new image
    image = new QGraphicsPixmapItem(QPixmap::fromImage(QImage(filename)));

    // remove all items before you add another item
    scene->clear();

    // Add the new image item
    scene->addItem(image);
}

void Widget::on_generateHistogram_clicked()
{

    if (this->checkifSceneEmpty()){
        QUrl serviceUrl = QUrl("http://0.0.0.0:9000/api/histogram");
        QNetworkAccessManager *manager = new QNetworkAccessManager(this);

        QNetworkRequest request(serviceUrl);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        QJsonObject jsonData;
        jsonData.insert("image_location", QJsonValue::fromVariant(ui->statusLabel->text()));


        QJsonDocument data(jsonData);
    //    qDebug() << data.toJson();

        connect(manager,
                SIGNAL(finished(QNetworkReply*)),
                this,
                SLOT(serviceRequestFinished(QNetworkReply*)));

        manager->post(request, data.toJson());
    }
    else{
        this->imageNotLoadedError();
    }
}

void Widget::faceDetectserviceRequestFinished(QNetworkReply *reply){

    QByteArray buffer = reply->readAll();
//    qDebug() << buffer;

    // convert buffer to object
    QJsonDocument jsonDoc(QJsonDocument::fromJson(buffer));
    QJsonObject jsonReply = jsonDoc.object();

    qDebug() << jsonReply;

    bool error = jsonReply["error"].toBool();


    if (!error){
        QJsonArray data = jsonReply["data"].toArray();
        qDebug() << data;
    }
    else{
        qDebug() << error;
    }
}

bool Widget::checkifSceneEmpty(){
    return !scene->items().empty();
//    return false;
}

void Widget::on_runFaceDetection_clicked()
{

    if (this->checkifSceneEmpty()){
        QUrl serviceUrl = QUrl("http://0.0.0.0:9000/api/face_detection");
        QNetworkAccessManager *manager = new QNetworkAccessManager(this);

        QNetworkRequest request(serviceUrl);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        QJsonObject jsonData;
        jsonData.insert("image_location", QJsonValue::fromVariant(ui->statusLabel->text()));

        QJsonDocument data(jsonData);

        connect(manager,
                SIGNAL(finished(QNetworkReply*)),
                this,
                SLOT(faceDetectserviceRequestFinished(QNetworkReply*)));

        manager->post(request, data.toJson());
    }
    else{
        this->imageNotLoadedError();
    }

}

void Widget::imageNotLoadedError(){
    QMessageBox::critical(this, "Image Error", "Image not loaded\n\nPlease take a new picture or load one");
}


void Widget::histogramGenerateRequestComplete(QNetworkReply *reply){

    QByteArray buffer = reply->readAll();
//    qDebug() << buffer;

    // convert buffer to object
    QJsonDocument jsonDoc(QJsonDocument::fromJson(buffer));
    QJsonObject jsonReply = jsonDoc.object();

    qDebug() << jsonReply;

    bool error = jsonReply["error"].toBool();

    if (!error){
        QJsonArray data = jsonReply["data"].toArray();
        qDebug() << data;
    }
    else{
        qDebug() << error;
    }
}
