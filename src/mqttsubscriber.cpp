#include "mqttsubscriber.h"
#include "mainwindow.h"

Subscriber::Subscriber(const QHostAddress& host,
                       const quint16 port,
                       const QString topic, QObject* parent)
    : QMQTT::Client(host, port, parent),topic_(topic), _qout(stdout)
{
    qDebug() << "Initializing subscriber.";
    qRegisterMetaType<Agents >("Agents");

    connect(this, &Subscriber::connected, this, &Subscriber::onConnected);
    connect(this, &Subscriber::subscribed, this, &Subscriber::onSubscribed);
    connect(this, &Subscriber::received, this, &Subscriber::onReceived);
    connect(this, SIGNAL(sendAgents(qint8, Agents)), MainWindow::instance(), SLOT(receiveAgents(qint16, Agents)));
}

Subscriber::~Subscriber()
{

}

void Subscriber::onConnected()
{
    _qout << "connected" << endl;
    subscribe(topic_, 0);
}

void Subscriber::onSubscribed(const QString& topic)
{
    _qout << "subscribed " << topic << endl;
}

void Subscriber::onReceived(const QMQTT::Message& message)
{
    _qout << "publish received: \"" << QString::fromUtf8(message.payload())
          << "\"" << endl;
}

