#include "osgwidget.h"

QtOSGWidget::QtOSGWidget(qreal scaleX, qreal scaleY, QWidget* parent)
    : QOpenGLWidget(parent)
    , _mGraphicsWindow(new osgViewer::GraphicsWindowEmbedded( this->x(), this->y(),
                                                              this->width(), this->height() ) )
    , _mViewer(new osgViewer::Viewer)
    , m_scaleX(scaleX)
    , m_scaleY(scaleY){
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
    _mViewer->getCamera()->setCullingMode(osg::CullSettings::NO_CULLING);
    _mViewer->home();


    _mViewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);
    _mViewer->realize();

}


QtOSGWidget::~QtOSGWidget(){

}

void QtOSGWidget::setScale(const qreal X, const qreal Y){
    m_scaleX = X;
    m_scaleY = Y;
    this->resizeGL(this->width(), this->height());
}

osg::Geometry* QtOSGWidget::getLaneGeometry(const std::vector<QLine> lanes){

    osg::Geometry* laneGeom = new osg::Geometry();
    osg::Vec3Array* laneVerts = new osg::Vec3Array;

    long vertex_count=0;
    for(const QLine lane:lanes){

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

void QtOSGWidget::addLines(std::vector<QLine > lanes, const qint16 type){

    double breadth=0.0;
    if(type==4){
        breadth=1.0;
    }
    else{
        breadth= 0.5;
    }

    for(const QLine lane:lanes){

        osg::Vec3 p1=osg::Vec3(lane.p1().x(),0.0f,lane.p1().y());
        osg::Vec3 p2=osg::Vec3(lane.p2().x(),0.0f,lane.p2().y());
        osg::Vec3 center=osg::Vec3(p1.x()+(p2.x()-p1.x())/2.0f,
                                   p1.y()+(p2.y()-p1.y())/2.0f,
                                   p1.z()+(p2.z()-p1.z())/2.0f);

        float dist = std::sqrt(std::pow(p1.x()-p2.x(), 2)
                               + std::pow(p1.y()-p2.y(), 2)
                               + std::pow(p1.z()-p2.z(), 2));

        //            std::cout << "dist --> " << dist << std::endl;
        osg::Box* box    = new osg::Box(center, dist, 0.01, breadth);

        osg::ShapeDrawable* sd = new osg::ShapeDrawable( box);

        if(type==4){
            sd->setColor( osg::Vec4(0.0f, 0.0f, 1.0f, 0.7f ) );
        }
        else{
            sd->setColor( osg::Vec4(1.0f, 0.0f, 1.0f, 0.7f ) );
        }

        osg::PositionAttitudeTransform* transform;
        transform = new osg::PositionAttitudeTransform();

        double angle=std::atan2(p1.z()-p2.z(), p1.x()-p2.x());

        //            std::cout << "angle --> " << angle << std::endl;
        osg::Quat q(0, osg::Vec3(0, 0, 0));
        q.makeRotate(-angle, 0, 1, 0);
        box->setRotation(q);

        osg::Geode* geode=new osg::Geode();
        geode->addDrawable(sd);
        root_->addChild(geode);
    }
}

void QtOSGWidget::addNetwork(const RoadNetwork& net){

    addLines(net.lanes_, 4);
    addLines(net.side_walks_, 5);
}

void QtOSGWidget::addModel(const Agent ag){
    //        std::cout << "adding agent " << ag.type << " " << ag.id << std::endl;

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
    else if(ag.type==4){
        sd->setColor( osg::Vec4(0.0f, 1.0f, 1.0f, 1.f ) );
    }
    else if(ag.type==5){
        sd->setColor( osg::Vec4(1.0f, 0.0f, 1.0f, 1.f ) );
    }
    else{
        sd->setColor( osg::Vec4(0.0f, 0.0f, 0.0f, 0.7f ) );
    }

    osg::PositionAttitudeTransform* transform;
    transform = new osg::PositionAttitudeTransform();
    osg::Vec3 position;
    //        qDebug() << ag.translate;
    position.set(ag.translate.x(),ag.translate.y(),ag.translate.z());

    transform->setPosition( position );
    osg::Geode* geode=new osg::Geode();
    geode->addDrawable(sd);

    transform->addChild(geode);
    root_->addChild(transform);

    model_transform_map_[name]=transform;
    model_geode_map_[name]=geode;
    QTime timer;
    timers_map_[name]=timer;
    timers_map_[name].restart();
}

void QtOSGWidget::addModels(const Agents ags){
    for(const Agent ag:ags){
        std::cout << "================ " << ag.type << " " << ag.id << std::endl;

        if(ag.type==1 && ag.id==1){
            addFP(ag);
            throw;
        }
        else{
            addModel(ag);
        }
    }
}

void QtOSGWidget::addFP(const Agent ag)
{
    //        std::cout << "adding agent " << ag.type << " " << ag.id << std::endl;

    osg::Node* fpNode = osgDB::readNodeFile("fpsphere.obj");

    QString name;
    name.append(QString::number(ag.type));
    name.append("_");
    name.append(QString::number(ag.id));

    osg::PositionAttitudeTransform* transform;
    transform = new osg::PositionAttitudeTransform();
    osg::Vec3 position;
    //        qDebug() << ag.translate;
    position.set(ag.translate.x(),ag.translate.y(),ag.translate.z());

    transform->setPosition( position );
    transform->addChild(fpNode);
    osg::Geode* geode=fpNode->asGeode();

    transform->addChild(geode);
    root_->addChild(transform);

    model_transform_map_[name]=transform;
    model_geode_map_[name]=geode;
    QTime timer;
    timers_map_[name]=timer;
    timers_map_[name].restart();
}

void QtOSGWidget::updateModel(const Agent ag){


    osg::Vec3 position;
    position.set(ag.translate.x(),ag.translate.y(),ag.translate.z());
    QString name;
    name.append(QString::number(ag.type));
    name.append("_");
    name.append(QString::number(ag.id));
    osg::PositionAttitudeTransform* transform=model_transform_map_[name];
    transform->setPosition( position );
    timers_map_[name].restart();
}

bool QtOSGWidget::modelExists(const Agent ag){
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

void QtOSGWidget::updateModels(const Agents ags){

    for(const Agent ag:ags){
        if(modelExists(ag)){
            updateModel(ag);
        }
        else{
            if(ag.type==1 && ag.id==1){
                addFP(ag);
            }
            else{
                addModel(ag);
            }
        }
    }
}

void QtOSGWidget::updateVisibility(){
    for(QString key : timers_map_.keys()){
        bool visible;
        if(timers_map_[key].elapsed()>2000.0){
            std::cout << "timeout ---> " << timers_map_[key].elapsed() << std::endl;
            visible=false;
        }
        else{
            visible=true;
        }
        qDebug() << key;
        if(key.compare(QString("1_1"))!=0){
            model_geode_map_[key]->setNodeMask(visible ? 0xffffffff : 0x0);
        }
    }
}

void QtOSGWidget::removeModels(const Agents ags){

    for(const Agent ag:ags){
        QString name;
        name.append(QString::number(ag.type));
        name.append("_");
        name.append(QString::number(ag.id));
        root_->removeChild(model_transform_map_[name]);
        model_transform_map_.remove(name);
        model_geode_map_.remove(name);
    }
}

void QtOSGWidget::updateScene(){
    this->update();
    paintGL();

}


void QtOSGWidget::paintGL() {
    _mViewer->frame();
}

void QtOSGWidget::resizeGL( int width, int height )
{
    this->getEventQueue()->windowResize(this->x()*m_scaleX, this->y() * m_scaleY, width*m_scaleX, height*m_scaleY);
    _mGraphicsWindow->resized(this->x()*m_scaleX, this->y() * m_scaleY, width*m_scaleX, height*m_scaleY);
    osg::Camera* camera = _mViewer->getCamera();
    camera->setViewport(0, 0, this->width()*m_scaleX, this->height()* m_scaleY);
}

void QtOSGWidget::initializeGL(){
    osg::Group* root = dynamic_cast<osg::Group*>(_mViewer->getSceneData());
    osg::StateSet* stateSet = root->getOrCreateStateSet();
    osg::Material* material = new osg::Material;
    material->setColorMode( osg::Material::AMBIENT_AND_DIFFUSE );
    stateSet->setAttributeAndModes( material, osg::StateAttribute::ON );
    stateSet->setMode( GL_DEPTH_TEST, osg::StateAttribute::ON );

}

void QtOSGWidget::mouseMoveEvent(QMouseEvent* event)
{
    this->getEventQueue()->mouseMotion(event->x()*m_scaleX, event->y()*m_scaleY);
}

void QtOSGWidget::mousePressEvent(QMouseEvent* event)
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

void QtOSGWidget::mouseReleaseEvent(QMouseEvent* event)
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

void QtOSGWidget::wheelEvent(QWheelEvent* event)
{
    int delta = event->delta();
    osgGA::GUIEventAdapter::ScrollingMotion motion = delta > 0 ?
                osgGA::GUIEventAdapter::SCROLL_UP : osgGA::GUIEventAdapter::SCROLL_DOWN;
    this->getEventQueue()->mouseScroll(motion);
}

bool QtOSGWidget::event(QEvent* event)
{
    bool handled = QOpenGLWidget::event(event);
    this->update();
    return handled;
}

osgGA::EventQueue* QtOSGWidget::getEventQueue() const {
    osgGA::EventQueue* eventQueue = _mGraphicsWindow->getEventQueue();
    return eventQueue;
}
