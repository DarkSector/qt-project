#ifndef HELLOWORLDWIDGET_H
#define HELLOWORLDWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>

namespace Ui {
class HelloWorldWidget;
}

class HelloWorldWidget : public QWidget
{
    // Macro: MOC meta object creator
    Q_OBJECT

public:
    explicit HelloWorldWidget(QWidget *parent = 0);
    ~HelloWorldWidget();

private slots:
    void on_pushButton_clicked();

private:
    Ui::HelloWorldWidget *ui;
    QPushButton *pushButton1;
    QLabel *label;
};

#endif // HELLOWORLDWIDGET_H
