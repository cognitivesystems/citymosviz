#include "mqttsubscriber.h"

MQTTSubscriber::MQTTSubscriber(const QHostAddress& host,
                       const quint16 port,
                       const QString topic, QObject* parent)
    : QMQTT::Client(host, port, parent),topic_(topic), _qout(stdout)
{
//    qDebug() << "Initializing subscriber.";
    qRegisterMetaType<Agents >("Agents");

    connect(this, &MQTTSubscriber::connected, this, &MQTTSubscriber::onConnected);
    connect(this, &MQTTSubscriber::subscribed, this, &MQTTSubscriber::onSubscribed);
}

MQTTSubscriber::~MQTTSubscriber()
{

}

void MQTTSubscriber::onConnected()
{
//    _qout << "connected" << endl;
    subscribe(topic_, 0);
}

void MQTTSubscriber::onSubscribed(const QString& topic)
{
//    _qout << "subscribed " << topic << endl;
}

