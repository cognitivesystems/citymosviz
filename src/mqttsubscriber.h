#ifndef MQTTSUBSCRIBER_H
#define MQTTSUBSCRIBER_H

#include <qmqtt.h>
#include <QTimer>
#include <QHostAddress>
#include <QMetaType>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include "agent.h"
#include <vector>

typedef std::vector<Agent > Agents;
Q_DECLARE_METATYPE(Agents);

enum SubType{
    vehicle=1,
    pedestrian=2
};


class MQTTSubscriber : public QMQTT::Client
{
    Q_OBJECT

public:
    MQTTSubscriber(const QHostAddress& host = QHostAddress::LocalHost,
                        const quint16 port = 1883,
                        const QString topic = "", QObject* parent = NULL);
    virtual ~MQTTSubscriber();

    QTextStream _qout;

public slots:
    void onConnected();

    void onSubscribed(const QString& topic);

    virtual void onReceived(const QMQTT::Message& message)=0;

private:
    QString topic_;
};

#endif // MQTTSUBSCRIBER_H
