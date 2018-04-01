#ifndef LOCALSERVER_H
#define LOCALSERVER_H

#include <QObject>
#include <QLocalServer>

class QLocalSocket;

class Localserver : public QLocalServer
{
    Q_OBJECT
public:
    explicit Localserver(QObject *parent = nullptr);
    ~Localserver(); // destructor added

private:
    QLocalSocket *localSocket;

public slots:
};

#endif // LOCALSERVER_H
