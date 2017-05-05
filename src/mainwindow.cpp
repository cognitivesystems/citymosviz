#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui_(new Ui::MainWindow), gen_(rdevice_()), dis_(1,255)
{
    MainWindow::singleton_ = this;
    setupGui();

    roads_.parse(QString("network.xml"));
    viewer_->addNetwork(roads_);

    cmos_.addSubscriber(SubType::pedestrian, QString("10.25.191.91"), 1883, "pedsim/update");
    cmos_.addSubscriber(SubType::vehicle, QString("10.25.191.170"), 1883, "citymos/vehicles/update");
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
//    qDebug() << "Received --> " << type << " " << data.size();

    static bool init_done=false;
    if(!init_done){
        viewer_->addModels(data);
        viewer_->updateScene();
        init_done=true;
    }
    else{
        viewer_->updateModels(data);
        viewer_->updateScene();
    }
}

