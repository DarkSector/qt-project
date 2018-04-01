#include "helloworldwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HelloWorldWidget w;
    w.show();

    return a.exec();
}
