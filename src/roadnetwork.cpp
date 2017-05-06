#include "roadnetwork.h"

RoadNetwork::RoadNetwork(QObject* parent){
}

RoadNetwork::~RoadNetwork(){
}

void RoadNetwork::parseRoads(const QString filename)
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

    for (int i = 0; i < lanes.size(); i++) {

        QDomNode n = lanes.item(i);
        QDomElement from = n.firstChildElement("fromPoint");
        QDomElement to = n.firstChildElement("toPoint");

        QDomElement from_lon=from.firstChildElement("x");
        QDomElement from_lat=from.firstChildElement("y");

        QDomElement to_lon=to.firstChildElement("x");
        QDomElement to_lat=to.firstChildElement("y");


        QLine lane;
        lane.setP1(QPoint( std::atoi(from_lon.toElement().text().toStdString().c_str()), std::atoi(from_lat.toElement().text().toStdString().c_str())));
        lane.setP2(QPoint( std::atoi(to_lon.toElement().text().toStdString().c_str()), std::atoi(to_lat.toElement().text().toStdString().c_str())));
        lanes_.push_back(lane);

    }

    xmlFile.close();
}

void RoadNetwork::parseSideWalk(const QString filename)
{
    QFile file;
    file.setFileName(filename);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString val=file.readAll();
    file.close();

    QJsonObject obj;
    QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
    obj = doc.object();

    QJsonArray jsonArray = obj["pedestrian_path_points"].toArray();

    qDebug() << "Array Size -- > " << jsonArray.size();

    QMap<qint16, QPoint> point_map;
    QMap<qint16, std::vector<qint16> > link_map;

    foreach (const QJsonValue & value, jsonArray) {
        QJsonObject p_obj = value.toObject();

        quint16 idx=p_obj["index"].toInt();
//        qDebug()<<idx;
        QPoint p;
        p.setX(p_obj["x"].toDouble());
        p.setY(p_obj["z"].toDouble());
        point_map[idx]=p;

        std::vector<qint16> links;
        QJsonArray link_array = p_obj["linked_indexes"].toArray();
        foreach (const QJsonValue & val, link_array) {
            QJsonObject l_obj = val.toObject();
            links.emplace_back(l_obj["end_index"].toInt());
        }
        link_map[idx]=links;
//       qDebug() <<  "id --> " << idx;
    }

    std::cout << "point_map size --> " << point_map.size() << std::endl;

    for(quint16 key : point_map.keys()){

        QPoint p=point_map[key];
        std::vector<qint16> connections=link_map[key];
        for(quint16 c:connections){
            QLine line_walk;
            line_walk.setP1(p);
            line_walk.setP2(point_map[c]);
            side_walks_.push_back(line_walk);
        }

    }

    std::cout << "side_walks_ ----> " <<side_walks_.size() << std::endl;

}
