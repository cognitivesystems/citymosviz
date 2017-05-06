#include <QOpenGLWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QDesktopWidget>
#include <QScreen>
#include <QtGlobal>
#include <QWindow>
#include <QVector3D>
#include <QMap>

#include <osg/ref_ptr>
#include <osgViewer/GraphicsWindow>
#include <osgViewer/Viewer>
#include <osg/Camera>
#include <osg/ShapeDrawable>
#include <osg/StateSet>
#include <osg/Material>
#include <osg/LineSegment>
#include <osg/Geometry>
#include <osgGA/EventQueue>
#include <osgGA/TrackballManipulator>
#include <osg/PositionAttitudeTransform>
#include "agent.h"
#include "roadnetwork.h"
#include <iostream>
#include <stdio.h>

#include "gnuplot-iostream.h"

class QtOSGWidget : public QOpenGLWidget
{
public:
    QtOSGWidget(qreal scaleX, qreal scaleY, QWidget* parent = 0)
        : QOpenGLWidget(parent)
        , _mGraphicsWindow(new osgViewer::GraphicsWindowEmbedded( this->x(), this->y(),
                                                                  this->width(), this->height() ) )
        , _mViewer(new osgViewer::Viewer)
        , m_scaleX(scaleX)
        , m_scaleY(scaleY)
    {
        root_ = new osg::Group();
        osg::Camera* camera = new osg::Camera;
        camera->setViewport( 0, 0, this->width(), this->height() );
        camera->setClearColor( osg::Vec4( 1.0f, 1.0f, 1.f, 1.f ) );
        float aspectRatio = static_cast<float>( this->width()) / static_cast<float>( this->height() );
        camera->setProjectionMatrixAsPerspective( 60.f, aspectRatio, 0.1f, 1000.f );
        camera->setGraphicsContext( _mGraphicsWindow );

        _mViewer->setCamera(camera);
        _mViewer->setSceneData(root_);

        osgGA::TrackballManipulator* manipulator = new osgGA::TrackballManipulator;
        manipulator->setAllowThrow( false );
        this->setMouseTracking(true);
        _mViewer->setCameraManipulator(manipulator);
        osg::Vec3 look_from, look_at, up;
        look_from = osg::Vec3(0,1000,0);
        look_at = osg::Vec3(0,0,0);
        up = osg::Vec3(0,1,0);
        _mViewer->getCameraManipulator()->setHomePosition( look_from, look_at, up, false );
        _mViewer->home();


        _mViewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);
        _mViewer->realize();

    }


    virtual ~QtOSGWidget(){}

    void setScale(qreal X, qreal Y)
    {
        m_scaleX = X;
        m_scaleY = Y;
        this->resizeGL(this->width(), this->height());
    }

    osg::Geometry* getLaneGeometry(const std::vector<QLine> lanes){

        osg::Geometry* laneGeom = new osg::Geometry();
        osg::Vec3Array* laneVerts = new osg::Vec3Array;

        long vertex_count=0;
        for(const QLine lane:lanes){

            //            qDebug() << lane.p1();
            //            qDebug() << lane.p2();

            laneVerts->push_back( osg::Vec3(lane.p1().x(), 0.0, lane.p1().y()) ); // start
            laneVerts->push_back( osg::Vec3(lane.p2().x(), 0.0, lane.p2().y()) ); // start

            osg::DrawElementsUInt* primitive=new osg::DrawElementsUInt(osg::PrimitiveSet::LINES, 0);
            primitive->push_back(vertex_count);
            primitive->push_back(vertex_count+1);
            laneGeom->addPrimitiveSet(primitive);
            vertex_count+=2;
        }

        laneGeom->setVertexArray(laneVerts);
        osg::Vec4Array* colors = new osg::Vec4Array;
        colors->push_back(osg::Vec4(0.0f, 0.0f, 0.0f, 1.0f) ); //index 0 blue-ish
        laneGeom->setColorArray(colors);
        laneGeom->setColorBinding(osg::Geometry::BIND_OVERALL);
        return laneGeom;
    }

    void addNetwork(const RoadNetwork& net){

        static int counter=0;
        for(const QLine lane:net.lanes_){

            osg::Vec3 p1=osg::Vec3(lane.p1().x(),0.0f,lane.p1().y());
            osg::Vec3 p2=osg::Vec3(lane.p2().x(),0.0f,lane.p2().y());
//            Agent a;
//            a.type=3;
//            a.id=counter++;
//            a.translate.setX(p1.x());
//            a.translate.setY(p1.y());
//            a.translate.setZ(p1.z());
//            addModel(a);
//            a.id=counter++;
//            a.translate.setX(p2.x());
//            a.translate.setY(p2.y());
//            a.translate.setZ(p2.z());
//            addModel(a);

            osg::Vec3 center=osg::Vec3(p1.x()+(p2.x()-p1.x())/2.0f,
                                       p1.y()+(p2.y()-p1.y())/2.0f,
                                       p1.z()+(p2.z()-p1.z())/2.0f);


            //            osg::Vec3 center=p1;

            float dist = std::sqrt(std::pow(p1.x()-p2.x(), 2)
                                   + std::pow(p1.y()-p2.y(), 2)
                                   + std::pow(p1.z()-p2.z(), 2));

            std::cout << "dist --> " << dist << std::endl;
            osg::Box* box    = new osg::Box(center, dist, 0.01, 1.0 );

            osg::ShapeDrawable* sd = new osg::ShapeDrawable( box);

            sd->setColor( osg::Vec4(0.0f, 0.0f, 1.0f, 0.7f ) );

            osg::PositionAttitudeTransform* transform;
            transform = new osg::PositionAttitudeTransform();

            std::cout << "center --> " << center.x() << " " << center.z() << std::endl;
            double angle=std::atan2(p1.z()-p2.z(), p1.x()-p2.x());

            std::cout << "angle --> " << angle << std::endl;
            osg::Quat q(0, osg::Vec3(0, 0, 0));
            q.makeRotate(-angle, 0, 1, 0);
            box->setRotation(q);

            osg::Geode* geode=new osg::Geode();
            geode->addDrawable(sd);
            root_->addChild(geode);

        }
    }

    void addModel(const Agent ag){
        std::cout << "adding agent " << ag.type << " " << ag.id << std::endl;

        QString name;
        name.append(QString::number(ag.type));
        name.append("_");
        name.append(QString::number(ag.id));

        float radius;
        if(name==QString("1_1")){
            radius=7.0f;
        }
        else{
            radius=3.0f;
        }

        osg::Sphere* sphere    = new osg::Sphere( osg::Vec3( 0.f, 0.f, 0.f ), radius );
        osg::ShapeDrawable* sd = new osg::ShapeDrawable( sphere );
        if(ag.type==0){
            sd->setColor( osg::Vec4(1.0f, 0.0f, 0.0f, 1.f ) );
        }
        else if(ag.type==1){
            sd->setColor( osg::Vec4(0.0f, 1.0f, 0.0f, 1.f ) );
        }
        else if(ag.type==2){
            sd->setColor( osg::Vec4(0.0f, 1.0f, 1.0f, 1.f ) );
        }
        else if(ag.type==3){
            sd->setColor( osg::Vec4(1.0f, 1.0f, 0.0f, 1.f ) );
        }
        else{
            sd->setColor( osg::Vec4(0.0f, 0.0f, 0.0f, 0.7f ) );
        }

        osg::PositionAttitudeTransform* transform;
        transform = new osg::PositionAttitudeTransform();
        osg::Vec3 position;
        qDebug() << ag.translate;
        position.set(ag.translate.x(),ag.translate.y(),ag.translate.z());

        transform->setPosition( position );
        osg::Geode* geode=new osg::Geode();
        geode->addDrawable(sd);
        transform->addChild(geode);
        root_->addChild(transform);

        model_transform_map_[name]=transform;
    }

    void addModels(const Agents ags){
        for(const Agent ag:ags){
            addModel(ag);
        }
    }

    void updateModel(const Agent ag){

        osg::Vec3 position;
        position.set(ag.translate.x(),ag.translate.y(),ag.translate.z());
        QString name;
        name.append(QString::number(ag.type));
        name.append("_");
        name.append(QString::number(ag.id));
        osg::PositionAttitudeTransform* transform=model_transform_map_[name];
        transform->setPosition( position );
        //        std::cout << "Update ---> " << ag.type << " " << ag.id << " " << position.x() << " " << position.y() << " " << position.z() << std::endl;
    }

    bool modelExists(const Agent ag){
        QString name;
        name.append(QString::number(ag.type));
        name.append("_");
        name.append(QString::number(ag.id));
        if(model_transform_map_[name]==NULL){
            return false;
        }
        else{
            return true;
        }
    }

    void updateModels(const Agents ags){

        for(const Agent ag:ags){
            if(modelExists(ag)){
                updateModel(ag);
            }
            else{
                addModel(ag);
            }
        }
        usleep(10000);
    }

    void removeModels(const Agents ags){

        for(const Agent ag:ags){
            QString name;
            name.append(QString::number(ag.type));
            name.append("_");
            name.append(QString::number(ag.id));
            root_->removeChild(model_transform_map_[name]);
            model_transform_map_.remove(name);
        }
    }

    void updateScene(){
        this->update();
        paintGL();

    }


protected:

    virtual void paintGL() {
        _mViewer->frame();
    }

    virtual void resizeGL( int width, int height )
    {
        this->getEventQueue()->windowResize(this->x()*m_scaleX, this->y() * m_scaleY, width*m_scaleX, height*m_scaleY);
        _mGraphicsWindow->resized(this->x()*m_scaleX, this->y() * m_scaleY, width*m_scaleX, height*m_scaleY);
        osg::Camera* camera = _mViewer->getCamera();
        camera->setViewport(0, 0, this->width()*m_scaleX, this->height()* m_scaleY);
    }

    virtual void initializeGL(){
        osg::Group* root = dynamic_cast<osg::Group*>(_mViewer->getSceneData());
        osg::StateSet* stateSet = root->getOrCreateStateSet();
        osg::Material* material = new osg::Material;
        material->setColorMode( osg::Material::AMBIENT_AND_DIFFUSE );
        stateSet->setAttributeAndModes( material, osg::StateAttribute::ON );
        stateSet->setMode( GL_DEPTH_TEST, osg::StateAttribute::ON );

    }

    virtual void mouseMoveEvent(QMouseEvent* event)
    {
        this->getEventQueue()->mouseMotion(event->x()*m_scaleX, event->y()*m_scaleY);
    }

    virtual void mousePressEvent(QMouseEvent* event)
    {
        unsigned int button = 0;
        switch (event->button()){
        case Qt::LeftButton:
            button = 1;
            break;
        case Qt::MiddleButton:
            button = 2;
            break;
        case Qt::RightButton:
            button = 3;
            break;
        default:
            break;
        }
        this->getEventQueue()->mouseButtonPress(event->x()*m_scaleX, event->y()*m_scaleY, button);
    }

    virtual void mouseReleaseEvent(QMouseEvent* event)
    {
        unsigned int button = 0;
        switch (event->button()){
        case Qt::LeftButton:
            button = 1;
            break;
        case Qt::MiddleButton:
            button = 2;
            break;
        case Qt::RightButton:
            button = 3;
            break;
        default:
            break;
        }
        this->getEventQueue()->mouseButtonRelease(event->x()*m_scaleX, event->y()*m_scaleY, button);
    }

    virtual void wheelEvent(QWheelEvent* event)
    {
        int delta = event->delta();
        osgGA::GUIEventAdapter::ScrollingMotion motion = delta > 0 ?
                    osgGA::GUIEventAdapter::SCROLL_UP : osgGA::GUIEventAdapter::SCROLL_DOWN;
        this->getEventQueue()->mouseScroll(motion);
    }

    virtual bool event(QEvent* event)
    {
        bool handled = QOpenGLWidget::event(event);
        this->update();
        return handled;
    }

private:

    osgGA::EventQueue* getEventQueue() const {
        osgGA::EventQueue* eventQueue = _mGraphicsWindow->getEventQueue();
        return eventQueue;
    }

    osg::Group* root_;

    osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> _mGraphicsWindow;
    osg::ref_ptr<osgViewer::Viewer> _mViewer;
    qreal m_scaleX, m_scaleY;

    QMap<QString, osg::PositionAttitudeTransform* > model_transform_map_;
};