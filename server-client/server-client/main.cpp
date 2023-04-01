#include "mainmultiplayer.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainMultiplayer w;
    w.show();
    return a.exec();
}
