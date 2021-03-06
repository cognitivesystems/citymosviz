#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGLWidget>
#include <QDebug>
#include <cmath>
#include <iostream>
#include "citymosinf.h"
#include "osgwidget.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <random>
#include <osgViewer/Viewer>
#include "roadnetwork.h"
#include <cmath>

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow{

    Q_OBJECT

public:
    virtual ~MainWindow();

    static MainWindow* instance();
    void setupGui();

public slots:
    void receiveAgents(const Agents& data);

protected:
    MainWindow(QWidget* parent = NULL);
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui_;

    static MainWindow* singleton_;

    QtOSGWidget* viewer_;

    CityMosInf cmos_;

    std::random_device rdevice_;
    std::mt19937 gen_;
    std::uniform_int_distribution<> dis_;

    QMap<QString, QColor> color_map_;

    bool pan_=true;
    int panStartX_, panStartY_;

    RoadNetwork roads_;
};

#endif // MAINWINDOW_H
