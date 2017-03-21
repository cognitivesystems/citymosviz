#ifndef CITYMOSINF_H
#define CITYMOSINF_H

#include "mqttsubscriber.h"
#include <QVector>

class CityMosInf: QObject
{
    Q_OBJECT

public:
    CityMosInf();
    ~CityMosInf();

    void addSubscriber(const QString ip, const quint16 port, const QString topic);

private:

    QVector<Subscriber* > subs_;

};

#endif // CITYMOSINF_H
