#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>

namespace Ui {
class MainWindow;
}

class LocalServer;

class MainWindow : public QMainWindow
{
    Q_OBJECT    

public:
    explicit MainWindow(QWidget *parent = 0);
    QString getServerName();
    ~MainWindow();

private slots:
    void on_exitButton_clicked();

    void on_submitButton_clicked();

    void on_startButton_clicked();

private:
    Ui::MainWindow *ui;
    LocalServer *mLocalServer;

};

#endif // MAINWINDOW_H
