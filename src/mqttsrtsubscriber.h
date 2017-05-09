#ifndef MQTTSRTSUBSCRIBER_H
#define MQTTSRTSUBSCRIBER_H

#include "mqttsubscriber.h"


class MQTTSRTSubscriber : public MQTTSubscriber
{
    Q_OBJECT

public:
    MQTTSRTSubscriber(const QHostAddress& host = QHostAddress::LocalHost,
                        const quint16 port = 1883,
                        const QString topic = "", QObject* parent = NULL);
    virtual ~MQTTSRTSubscriber();

public slots:
    void onReceived(const QMQTT::Message& message);

signals:
    void sendAgents(const Agents&);
};

#endif // MQTTSRTSUBSCRIBER_H
