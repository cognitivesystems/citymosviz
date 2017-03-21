#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGLWidget>
#include <QDebug>
#include <QWindow>
#include <cmath>
#include <iostream>
#include "citymosinf.h"
#include "ui_mainwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow{

    Q_OBJECT

public:

    virtual ~MainWindow();

    static MainWindow* instance();

    //    void initializeGL();

    //    void qgluPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);

    //    /// @note camera decides renderer size
    //    void resizeGL(int width, int height);

    //    void paintGL();

public slots:
    void receiveAgents(const qint16& type, const Agents& data);

protected:
    MainWindow(QWidget* parent = NULL);

    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;

    static MainWindow* singleton_;

    CityMosInf cmos;

};

#endif // MAINWINDOW_H
