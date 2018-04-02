#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

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
    ~Widget();

private slots:
    void on_tabWidget_currentChanged(int index);

    void on_applicationClose_clicked();

    void on_takePicture_clicked();

    void on_browseButton_clicked();

private:
    Ui::Widget *ui;
    QCamera *pCamera;
    QCameraViewfinder *pCameraViewfinder;
    QCameraImageCapture *pCameraImageCapture;
    QVBoxLayout *pLayout;
};

#endif // WIDGET_H
