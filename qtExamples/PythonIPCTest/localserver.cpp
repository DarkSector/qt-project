#include "localserver.h"
#include <QLocalSocket>

LocalServer::LocalServer(QObject *parent)
    : QLocalServer(parent)
{
    localSocket = nullptr;
    connect(this, &LocalServer::newConnection, [&](){
        localSocket = nextPendingConnection();
    });
}

void LocalServer::submit(const QString &myString){
    // Method is kill
    if (localSocket){
        // only stream if a socket exists
        QTextStream T(localSocket);
        T << myString;
        localSocket->flush();
    }

}

LocalServer::~LocalServer(){
    // destructor method
}
