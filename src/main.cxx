#include <QApplication>
#include <QLabel>
#include <QWidget>

#include "mainwin.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    //hello.resize(400, 400);
    return app.exec();
}

