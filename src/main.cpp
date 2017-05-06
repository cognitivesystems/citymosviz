#include <iostream>
#include <QApplication>
#include <stdexcept>
#include "mainwindow.h"

MainWindow* MainWindow::singleton_ = NULL;

int
main(int argc, char** argv){
    try{

        QApplication app(argc, argv);

        QObject::connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));

        MainWindow::instance()->show();

        return app.exec();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return -1;
    }
}


//#include <QApplication>

//#include <Inventor/nodes/SoBaseColor.h>
//#include <Inventor/nodes/SoCone.h>
//#include <Inventor/nodes/SoSeparator.h>

//#include <Quarter/Quarter.h>
//#include <Quarter/QuarterWidget.h>

//using namespace SIM::Coin3D::Quarter;

//int
//main(int argc, char ** argv)
//{
//  QApplication app(argc, argv);
//  // Initializes Quarter library (and implicitly also the Coin and Qt
//  // libraries).
//  Quarter::init();

//  // Make a dead simple scene graph by using the Coin library, only
//  // containing a single yellow cone under the scenegraph root.
//  SoSeparator * root = new SoSeparator;
//  root->ref();

//  SoBaseColor * col = new SoBaseColor;
//  col->rgb = SbColor(1, 1, 0);
//  root->addChild(col);

//  root->addChild(new SoCone);

//  // Create a QuarterWidget for displaying a Coin scene graph
//  QuarterWidget * viewer = new QuarterWidget;
//  viewer->setSceneGraph(root);

//  // make the viewer react to input events similar to the good old
//  // ExaminerViewer
//  viewer->setNavigationModeFile(QUrl("coin:///scxml/navigation/examiner.xml"));

//  // Pop up the QuarterWidget
//  viewer->show();
//  // Loop until exit.
//  app.exec();
//  // Clean up resources.
//  root->unref();
//  delete viewer;

//  Quarter::clean();

//  return 0;
//}
