#include <QApplication>
#include <QLabel>
#include <QWidget>

#include "mainwin.h"

int main(int argc, char **argv)
{
    QCoreApplication::setOrganizationName("MySoft");
    QCoreApplication::setOrganizationDomain("mysoft.com");
    QCoreApplication::setApplicationName("MDIO Register Set");
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}

