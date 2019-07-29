#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    
    QSettings *settings = new QSettings("settings.ini",QSettings::IniFormat);
    settings->setValue("settings/port",1252);
    settings->setValue("settings/IP", "127.0.0.1");
    settings->sync();
    
    w.setWindowTitle("Serverless-Chat");
    w.show();

    return a.exec();
}
