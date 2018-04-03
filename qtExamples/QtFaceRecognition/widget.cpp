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
#include <QProcess>
#include <QPointF>

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>

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

    histogramScene = new QGraphicsScene(this);
    ui->histogramGraphicsView->setScene(histogramScene);

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
    QMessageBox::StandardButton reply = QMessageBox::question(this,
                                                               tr("Close application"),
                                                               tr("Are you sure?"),
                                                               QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes){
        close();
    }

}

void Widget::onImageSaved(int id, const QString &fileName){

    // Question message content
    QString name = QString("Image saved at\n\n"
                           "%1\n\n"
                           "Would you like to use this? Press Yes to proceed or No to go back").arg(fileName);

    // Message box reply comes back
    QMessageBox::StandardButton reply = QMessageBox::question(this,
                                                              tr("Image saved!"),
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

    QString status = QString("Image added!");
    updateCurrentStatus(status, false);
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
                SLOT(histogramGenerateRequestComplete(QNetworkReply*)));

        manager->post(request, data.toJson());
        updateCurrentStatus("Sending request ...", false);
    }
    else{
        this->imageNotLoadedError();
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
        updateCurrentStatus("Sending request ...", false);
    }
    else{
        QMessageBox::StandardButton reply = QMessageBox::information(this,
                                                                     tr("Feature detection"),
                                                                     tr("Please load an image before processing\n\nLoad now?"),
                                                                     QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes){
            this->on_loadImage_clicked();
        }

    }

}

void Widget::imageNotLoadedError(){
    QMessageBox::critical(this, tr("Image Error"), tr("Image not loaded\n\nPlease take a new picture or load one"));
}

void Widget::histogramGenerateRequestComplete(QNetworkReply *reply){

    // Charts need namespace resolving otherwise error creeps up
    QT_CHARTS_USE_NAMESPACE

    // get the reply buffer and read it completely into QByteArray
    QByteArray buffer = reply->readAll();
//    qDebug() << buffer;

    if (!buffer.isEmpty()){
        ui->tabWidget->setCurrentIndex(2);
        // convert buffer to object
        QJsonDocument jsonDoc(QJsonDocument::fromJson(buffer));
        QJsonObject jsonReply = jsonDoc.object();

        // check for boolean error
        bool error = jsonReply["error"].toBool();

        if (!error){
            // This means data came back with actual values
            QJsonObject data = jsonReply["data"].toObject();
            // extact rgb hist values
            QJsonArray r = data["r"].toArray();
            QJsonArray g = data["g"].toArray();
            QJsonArray b = data["b"].toArray();

            updateCurrentStatus(QString("\n\nRequest Completed\n\nHistogram generated"), true);

            // line chart generation

            // three lines
            QLineSeries *series0 = new QLineSeries();
            QLineSeries *series1 = new QLineSeries();
            QLineSeries *series2 = new QLineSeries();

            // Set color for lines
            series0->setColor(QColor(255, 0, 0, 255));
            series1->setColor(QColor(0, 255, 0, 255));
            series2->setColor(QColor(0, 0, 255, 255));

            // load dataset
            for(int i = 0; i <= 255; i++){
                // is the pixel intensity
                // y is the pixel count
                series0->append(i, r[i].toDouble());
                series1->append(i, g[i].toDouble());
                series2->append(i, b[i].toDouble());
            }

            QChart *chart = new QChart();

            // hide the legend
            chart->legend()->hide();

            // add the three series
            chart->addSeries(series0);
            chart->addSeries(series1);
            chart->addSeries(series2);

            // x-y AXIS update and title update
            chart->createDefaultAxes();
            chart->axisX()->setRange(1, 256);
            chart->axisX()->setTitleText("PIXEL INTENSITY");
            chart->axisY()->setTitleText("PIXEL COUNT");
            chart->setTitle("RGB PIXEL INTENSITY DISTRIBUTION HISTOGRAM");

            // create a chart view widget
            QChartView *chartView = new QChartView(chart);
            chartView->setRenderHint(QPainter::Antialiasing);

            // resize it to fill out the entire graphicsview
            chartView->resize(ui->histogramGraphicsView->width(), ui->histogramGraphicsView->height());

            // remove existing data if any and then add the histogram chart to the scene
            histogramScene->clear();
            histogramScene->addWidget(chartView);

            // move over to the tab once it's done
            ui->tabWidget->setCurrentIndex(2);

        }
        else{
            // This is error
            qDebug() << "Error encountered";
            qDebug() << error;
            updateCurrentStatus(QString("\n\nRequest Completed\n\nNo images detected"), true);
        }
    }

    else{
        QMessageBox::critical(this, tr("Error"), tr("No response from server\nPlease make sure the server is running"
                                             "before feature detection"));
    }
}

void Widget::faceDetectserviceRequestFinished(QNetworkReply *reply){

    QByteArray buffer = reply->readAll();
    qDebug() << buffer;

    if (!buffer.isEmpty()){


        // convert buffer to object
        QJsonDocument jsonDoc(QJsonDocument::fromJson(buffer));
        QJsonObject jsonReply = jsonDoc.object();

        qDebug() << jsonReply;

        bool error = jsonReply["error"].toBool();


        if (!error){
            // This means data came back with actual values
            QJsonArray data = jsonReply["data"].toArray();
            qDebug() << "About to iterate over object";

            for(auto v: data){
                QJsonArray arr = v.toArray();
                QGraphicsRectItem *rect = new QGraphicsRectItem();
                rect->setRect(arr[0].toInt(), arr[1].toInt(), arr[2].toInt(), arr[3].toInt());
                scene->addItem(rect);
            }
            updateCurrentStatus(QString("\n\nRequest Completed\n\nFaces detected"), true);
        }
        else{
            // This is error
            qDebug() << "Error encountered";
            qDebug() << error;
            updateCurrentStatus(QString("\n\nRequest Completed\n\nNo images detected"), true);
        }
    }

    else{
        QMessageBox::critical(this, tr("Error"), tr("No response from server\nPlease make sure the server is running"
                                             "before feature detection"));
    }

}

void Widget::updateCurrentStatus(QString status, bool append=false){
    qDebug() << "updating current status";
    if (append){
        QString currentText = ui->currentStatus->text();
        currentText.append(status);
        ui->currentStatus->setText(currentText);
    }
    else{
        ui->currentStatus->setText(status);
    }

}
