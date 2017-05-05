#ifndef MQTTVEHICLESUBSCRIBER_H
#define MQTTVEHICLESUBSCRIBER_H

#include "mqttsubscriber.h"


class MQTTVehicleSubscriber : public MQTTSubscriber
{
    Q_OBJECT

public:
    MQTTVehicleSubscriber(const QHostAddress& host = QHostAddress::LocalHost,
                        const quint16 port = 1883,
                        const QString topic = "", QObject* parent = NULL);
    virtual ~MQTTVehicleSubscriber();

public slots:
    void onReceived(const QMQTT::Message& message);

signals:
    void sendAgents(const Agents&);
};

#endif // MQTTVEHICLESUBSCRIBER_H
