#ifndef MQTTPEDESTRIANSUBSCRIBER_H
#define MQTTPEDESTRIANSUBSCRIBER_H

#include "mqttsubscriber.h"


class MQTTPedestrianSubscriber : public MQTTSubscriber
{
    Q_OBJECT

public:
    MQTTPedestrianSubscriber(const QHostAddress& host = QHostAddress::LocalHost,
                        const quint16 port = 1883,
                        const QString topic = "", QObject* parent = NULL);
    virtual ~MQTTPedestrianSubscriber();

public slots:
    void onReceived(const QMQTT::Message& message);

signals:
    void sendAgents(const Agents&);
};

#endif // MQTTPEDESTRIANSUBSCRIBER_H
