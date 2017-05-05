#ifndef CITYMOSINF_H
#define CITYMOSINF_H

#include "mqttvehiclesubscriber.h"
#include "mqttpedestriansubscriber.h"
#include <QVector>

class CityMosInf: QObject
{
    Q_OBJECT

public:
    CityMosInf();
    ~CityMosInf();

    void addSubscriber(const SubType type, const QString ip, const quint16 port, const QString topic);

private:

    QVector<MQTTSubscriber* > subs_;

};

#endif // CITYMOSINF_H
