#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui_(new Ui::MainWindow), gen_(rdevice_()), dis_(1,255)
{
    MainWindow::singleton_ = this;
    setupGui();

    roads_.parseRoads(QString("road.xml"));
    roads_.parseSideWalk(QString("sidewalk_points.json"));

    cmos_.addSubscriber(SubType::vehicle, QString("10.25.191.170"), 1883, "citymos/vehicles/update");
    cmos_.addSubscriber(SubType::pedestrian, QString("10.25.191.170"), 1883, "pedsim/update");

}

MainWindow::~MainWindow()
{
    MainWindow::singleton_ = NULL;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    std::cout << "Closing gui" << std::endl;
}


MainWindow*
MainWindow::instance()
{
    if (NULL == MainWindow::singleton_)
    {
        new MainWindow();
    }

    return MainWindow::singleton_;
}

void MainWindow::setupGui()
{
    ui_->setupUi(this);

    viewer_ = new QtOSGWidget(1, 1, MainWindow::instance());

    ui_->gridLayout_main->addWidget(viewer_, 0, 0);
    this->setWindowIconText("CityMos Viz");
    this->setWindowTitle("SCityMos Viz");
}

void MainWindow::receiveAgents(const Agents &data)
{
    static bool init_done=false;

    if(!init_done){
        viewer_->addNetwork(roads_);
        init_done=true;
    }

    viewer_->updateModels(data);
    viewer_->updateVisibility();
    viewer_->updateScene();
}

