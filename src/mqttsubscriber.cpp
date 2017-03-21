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
    QObject::connect(this, SIGNAL(sendAgents(qint16, Agents)), MainWindow::instance(), SLOT(receiveAgents(qint16, Agents)));
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

    //_qout << "publish received: \"";// << QString::fromUtf8(message.payload())
    //<< "\"" << endl;

    QJsonObject obj;
    QJsonDocument doc = QJsonDocument::fromJson(message.payload());
    obj = doc.object();

    //qDebug() << obj;

    QJsonArray jsonArray = obj["pedestrians"].toArray();

    qDebug() << jsonArray.size();

    Agents agents_data;
    foreach (const QJsonValue & value, jsonArray) {
        QJsonObject p_obj = value.toObject();

        Agent a;
        a.id=p_obj["id"].toInt();
        QJsonObject tr_array = p_obj["translate"].toObject();
        a.setPosition(QVector3D(tr_array["x"].toDouble(), tr_array["y"].toDouble(), tr_array["z"].toDouble()));
        QJsonObject v_array = p_obj["velocity"].toObject();
        a.setVelocity(QVector3D(v_array["x"].toDouble(), v_array["y"].toDouble(), v_array["z"].toDouble()));

        agents_data.push_back(a);
    }

    qint16 type =0;
    qDebug()<< "Sending agents --> " << type << " " << agents_data.size();

    sendAgents(type, agents_data);

}

