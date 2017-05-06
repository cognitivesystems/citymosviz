#include "roadnetwork.h"

RoadNetwork::RoadNetwork(QObject* parent){
}

RoadNetwork::~RoadNetwork(){
}

void RoadNetwork::parse(const QString filename)
{

    //load the file
    QFile xmlFile(filename);
    if (!xmlFile.exists() || !xmlFile.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "Check your file";
        return;
    }
    QDomDocument doc;
    doc.setContent(&xmlFile);

    QDomNodeList lanes = doc.elementsByTagName("lane");
//    qDebug() << lanes.size();

    for (int i = 0; i < lanes.size(); i++) {

        QDomNode n = lanes.item(i);
        QDomElement from = n.firstChildElement("fromPoint");
        QDomElement to = n.firstChildElement("toPoint");

        QDomElement from_lon=from.firstChildElement("x");
        QDomElement from_lat=from.firstChildElement("y");

        QDomElement to_lon=to.firstChildElement("x");
        QDomElement to_lat=to.firstChildElement("y");


        QLine lane;
//        qDebug() << from_lon.toElement().text();
//        qDebug() << from_lat.toElement().text();
//        qDebug() << to_lon.toElement().text();
//        qDebug() << to_lat.toElement().text();

//        qDebug() << std::atoi(from_lon.toElement().text().toStdString().c_str());
//        qDebug() << std::atoi(from_lat.toElement().text().toStdString().c_str());
//        qDebug() << std::atoi(to_lon.toElement().text().toStdString().c_str());
//        qDebug() << std::atoi(to_lat.toElement().text().toStdString().c_str());

        lane.setP1(QPoint( std::atoi(from_lon.toElement().text().toStdString().c_str()), std::atoi(from_lat.toElement().text().toStdString().c_str())));
        lane.setP2(QPoint( std::atoi(to_lon.toElement().text().toStdString().c_str()), std::atoi(to_lat.toElement().text().toStdString().c_str())));
        lanes_.push_back(lane);

//        qDebug() << lane.p1();
//        qDebug() << lane.p2();

//        if(i==0){
//            break;
//        }

    }

    xmlFile.close();
}
