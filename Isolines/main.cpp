#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName("Alexander Likhachev");
    app.setApplicationName("Isolines");
    MainWindow w;
    w.show();

    return app.exec();
}
