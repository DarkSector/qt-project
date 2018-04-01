#ifndef LOCALSERVER_H
#define LOCALSERVER_H

#include <QObject>

class Localserver : public QObject
{
    Q_OBJECT
public:
    explicit Localserver(QObject *parent = nullptr);

signals:

public slots:
};

#endif // LOCALSERVER_H