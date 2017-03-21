#include "citymosinf.h"

CityMosInf::CityMosInf()
{
    subs_.clear();
}


CityMosInf::~CityMosInf(){

}

void CityMosInf::addSubscriber(const QString ip, const quint16 port, const QString topic)
{
    subs_.push_back(new Subscriber(QHostAddress::LocalHost, 1883, topic));
    subs_.back()->connectToHost();

}

