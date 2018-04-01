#include "localserver.h"
#include <QLocalServer>

Localserver::Localserver(QObject *parent)
    : QLocalServer(parent)
{

}

Localserver::~Localserver(){
    // destructor method
}
