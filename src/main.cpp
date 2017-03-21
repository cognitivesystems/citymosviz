#include <iostream>
#include <QApplication>
#include <stdexcept>
#include <ros/ros.h>
#include "mainwindow.h"

MainWindow* MainWindow::singleton_ = NULL;

int
main(int argc, char** argv)
{

    try
    {
        QApplication application(argc, argv);

        QObject::connect(&application, SIGNAL(lastWindowClosed()), &application, SLOT(quit()));

        MainWindow::instance()->resize(640,480);
        MainWindow::instance()->show();

        return application.exec();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return -1;
    }
}
