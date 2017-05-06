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
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

class RoadNetwork : public QObject
{
    Q_OBJECT
public:
    RoadNetwork(QObject* parent = 0);
    ~RoadNetwork();

    void parseRoads(const QString filename);
    void parseSideWalk(const QString filename);

public:
    std::vector<QLine > lanes_;
    std::vector<QLine > side_walks_;

};


#endif // ROADNETWORK_H
