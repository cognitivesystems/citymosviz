#ifndef OSGWIDGET_H
#define OSGWIDGET_H

#include <QOpenGLWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QDesktopWidget>
#include <QVector3D>
#include <QMap>
#include <QTime>

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
#include <osg/CullSettings>
#include <osgGA/TrackballManipulator>
#include <osg/PositionAttitudeTransform>
#include <iostream>
#include "defs.h"
#include "roadnetwork.h"

class QtOSGWidget : public QOpenGLWidget
{
public:
    QtOSGWidget(qreal scaleX, qreal scaleY, QWidget* parent = 0);
    virtual ~QtOSGWidget();

    void setScale(const qreal X, const qreal Y);

    osg::Geometry* getLaneGeometry(const std::vector<QLine> lanes);

    void addNetwork(const RoadNetwork& net);

    void addModels(const Agents ags);
    void updateModels(const Agents ags);
    void updateVisibility();

    void removeModels(const Agents ags);
    void updateScene();

protected:
    void addLines(std::vector<QLine > lanes, const qint16 type);
    void addModel(const Agent ag);
    void updateModel(const Agent ag);
    bool modelExists(const Agent ag);

    virtual void paintGL();
    virtual void resizeGL( int width, int height );
    virtual void initializeGL();

    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);
    virtual void wheelEvent(QWheelEvent* event);
    virtual bool event(QEvent* event);

private:

    osgGA::EventQueue* getEventQueue() const;

    osg::Group* root_;

    osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> _mGraphicsWindow;
    osg::ref_ptr<osgViewer::Viewer> _mViewer;
    qreal m_scaleX, m_scaleY;

    QMap<QString, osg::PositionAttitudeTransform* > model_transform_map_;
    QMap<QString, osg::Geode* > model_geode_map_;

    QMap<QString, QTime> timers_map_;
};

#endif // OSGWIDGET_H

