#ifndef ROADNETWORK_H
#define ROADNETWORK_H

#include <QObject>
#include <QMetaType>
#include <QVector3D>
#include <QLine>
#include <QDomDocument>
#include <QFile>
#include <QDebug>
#include <iostream>
#include <vector>
#include <QLine>

class RoadNetwork : public QObject
{
    Q_OBJECT
public:
    RoadNetwork(QObject* parent = 0);
    ~RoadNetwork();

    void parse(const QString filename);

public:
    std::vector<QLine > lanes_;
};


#endif // ROADNETWORK_H
