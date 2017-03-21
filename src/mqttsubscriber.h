#ifndef MQTTSUBSCRIBER_H
#define MQTTSUBSCRIBER_H

#include <qmqtt.h>
#include <QTimer>
#include <QHostAddress>
#include <QMetaType>
#include "citymosinf.h"
#include "agent.h"
#include <vector>

typedef std::vector<Agent> Agents;
Q_DECLARE_METATYPE(Agents);

class Subscriber : public QMQTT::Client
{
    Q_OBJECT

public:
    explicit Subscriber(const QHostAddress& host = QHostAddress::LocalHost,
                        const quint16 port = 1883,
                        const QString topic = "", QObject* parent = NULL);
    virtual ~Subscriber();

    QTextStream _qout;

public slots:
    void onConnected();

    void onSubscribed(const QString& topic);

    void onReceived(const QMQTT::Message& message);

signals:
    void sendAgents(const qint8& type, const Agents&);

private:
    QString topic_;
};

#endif // MQTTSUBSCRIBER_H
