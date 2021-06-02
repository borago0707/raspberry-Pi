#include <QApplication>
#include "mainwindow.h"
#include "mobileplatform.h"

int main(int argc, char *argv[])
{

    if (wiringPiSetup() == -1)
    {
       return false;
    }

    QApplication a(argc, argv);

    MobilePlatform *mobile_platform = new MobilePlatform();
    MainWindow* mainWindow = new MainWindow(mobile_platform);
    mainWindow->showNormal();

    delete mainWindow;
    delete mobile_platform;
    return a.exec();
}
