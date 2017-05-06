#include "citymosinf.h"

CityMosInf::CityMosInf()
{
    subs_.clear();
}


CityMosInf::~CityMosInf(){

    subs_.erase(subs_.begin(), subs_.end());
    subs_.clear();
}

void CityMosInf::addSubscriber(const SubType type, const QString ip, const quint16 port, const QString topic)
{
    QHostAddress addr(ip);
//    qDebug() << ip;
//    qDebug() << port;
    if(type==SubType::vehicle){
        subs_.push_back(new MQTTVehicleSubscriber(addr, port, topic));
        subs_.back()->connectToHost();
    }
    else if(type==SubType::pedestrian){
        subs_.push_back(new MQTTPedestrianSubscriber(addr, port, topic));
        subs_.back()->connectToHost();
    }

}

