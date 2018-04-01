#include "localserver.h"
#include <QLocalSocket>

Localserver::Localserver(QObject *parent)
    : QLocalServer(parent)
{
    localSocket = nullptr;
    connect(this, &Localserver::newConnection, [&](){
        localSocket = nextPendingConnection();
    });
}

Localserver::~Localserver(){
    // destructor method
}
