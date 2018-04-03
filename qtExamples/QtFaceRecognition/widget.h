#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>
#include <QNetworkReply>

namespace Ui {
class Widget;
}

class QCamera;
class QCameraViewfinder;
class QCameraImageCapture;
class QVBoxLayout;

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);    
    void setEncoding();
    bool checkifSceneEmpty();
    void imageNotLoadedError();
    void updateCurrentStatus(QString, bool);
    ~Widget();

private slots:
    void on_tabWidget_currentChanged(int index);

    void on_applicationClose_clicked();

    void on_takePicture_clicked();

    void on_browseButton_clicked();

    void onImageSaved(int id, const QString &fileName);

    void on_loadImage_clicked();

    void on_generateHistogram_clicked();

    void faceDetectserviceRequestFinished(QNetworkReply *reply);

    void histogramGenerateRequestComplete(QNetworkReply *reply);

    void on_runFaceDetection_clicked();

private:
    Ui::Widget *ui;

    // Camera
    QCamera *pCamera;
    QCameraViewfinder *pCameraViewfinder;
    QCameraImageCapture *pCameraImageCapture;
    QVBoxLayout *pLayout;

    // Graphics
    QGraphicsScene *scene;
    QGraphicsRectItem *rect;
    QGraphicsPixmapItem *image;    


};

#endif // WIDGET_H
