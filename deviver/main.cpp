#include "mywidget.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    myWidget w;
    w.showMaximized();

    return a.exec();
}
