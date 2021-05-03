#include <QApplication>
#include <QLabel>
#include <QWidget>

#include "mainwin.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}

