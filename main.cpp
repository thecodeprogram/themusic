#include "mainwindow.h"
#include <QApplication>

#include <QDesktopWidget>
#include <QVariant>
#include <QSettings>
#include <QCommandLineParser>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setApplicationVersion(QT_VERSION_STR);
    QCoreApplication::setApplicationName(QLatin1String("The Music"));
    QCoreApplication::setOrganizationName(QLatin1String("TheCodeProgram"));

    MainWindow w;
    w.show();

    return app.exec();
}
