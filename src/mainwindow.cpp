#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    MainWindow::singleton_ = this;

    ui->setupUi(this);
    cmos.addSubscriber(QString("localhost"), 1883, "pedsim/update");

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

void MainWindow::receiveAgents(const qint16 &type, const Agents &data)
{
    qDebug() << "Received --> " << type << " " << data.size();

    for ( const Agent agent_data : data) {
        qDebug() << agent_data.id;
        qDebug() << agent_data.translate;
        qDebug() << agent_data.velocity;
    }


}

//void MainWindow::initializeGL(){
//    /// In modelview hand is at origin
//    glClearColor(1.0, 1.0, 1.0, 1.0); ///< essential using white, as labels start from 0!!!!!         glEnable(GL_DEPTH_TEST);         glDisable(GL_LIGHTING);         glDisable(GL_DOUBLEBUFFER);         glEnableClientState(GL_VERTEX_ARRAY);                  /// Output opengl version         qDebug("OpenGL %d.%d",this->format().majorVersion(),this->format().minorVersion());
//}

//void MainWindow::qgluPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar){
//    const GLdouble ymax = zNear * tan(fovy * M_PI / 360.0);
//    const GLdouble ymin = -ymax;
//    const GLdouble xmin = ymin * aspect;
//    const GLdouble xmax = ymax * aspect;
//    glFrustum(xmin, xmax, ymin, ymax, zNear, zFar);
//}

///// @note camera decides renderer size
//void MainWindow::resizeGL(int width, int height){
//    if (height==0) height=1;
//    glViewport(0,0,width,height);
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    qgluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//}

//void MainWindow::paintGL(){
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//    glClear(GL_COLOR_BUFFER_BIT);

//    glBegin(GL_POLYGON);
//    glVertex2f(-0.5, -0.5);
//    glVertex2f(-0.5, 0.5);
//    glVertex2f(0.5, 0.5);
//    glVertex2f(0.5, -0.5);
//    glEnd();
//}

