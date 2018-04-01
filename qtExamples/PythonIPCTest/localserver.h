#ifndef LOCALSERVER_H
#define LOCALSERVER_H

#include <QObject>
#include <QLocalServer>

class QLocalSocket;

class LocalServer : public QLocalServer
{
    Q_OBJECT
public:
    explicit LocalServer(QObject *parent = nullptr);    
    ~LocalServer(); // destructor added
    void submit(const QString &myString);

private:
    QLocalSocket *localSocket;

public slots:
};

#endif // LOCALSERVER_H
