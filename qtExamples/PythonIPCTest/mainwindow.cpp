#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QDebug>
#include <QString>
#include "localserver.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mLocalServer = new LocalServer(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_exitButton_clicked()
{   
    QLocalServer::removeServer("myLocalServer");
    close();

}

void MainWindow::on_submitButton_clicked()
{

    mLocalServer->submit(ui->lineEdit->text());

}

void MainWindow::on_startButton_clicked()
{
    qDebug() << myServerName1;
    QString myServerName =  "myLocalServer";
    //https://stackoverflow.com/questions/15635215/not-able-to-start-qlocalserver
    QLocalServer::removeServer(myServerName);

    if(!mLocalServer->isListening()){
        // If not already listening
        if(!mLocalServer->listen(myServerName)){
            // start listening to myLocalServer socket
            QMessageBox::critical(this, "Error", mLocalServer->errorString());
            // if something goes wrong popup an error with actual reason
        }
        else{
            QString serverStatusMessage = QString("Server has already been started at \n%1").arg(mLocalServer->fullServerName());
            QMessageBox::information(this, "Server status", serverStatusMessage);
            // otherwise just tell the user it has been started
        }
    }
    else{
        // otherwise tell them you're listening
        QMessageBox::information(this, "Server status", "Server is already listening");
    }

}
