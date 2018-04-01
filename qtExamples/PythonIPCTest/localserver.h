#ifndef LOCALSERVER_H
#define LOCALSERVER_H

#include <QObject>
#include <QLocalServer>

class Localserver : public QLocalServer
{
    Q_OBJECT
public:
    explicit Localserver(QObject *parent = nullptr);
    ~Localserver(); // destructor added

signals:

public slots:
};

#endif // LOCALSERVER_H
