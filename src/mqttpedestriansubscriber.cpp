#include "mqttpedestriansubscriber.h"
#include "mainwindow.h"

MQTTPedestrianSubscriber::MQTTPedestrianSubscriber(const QHostAddress& host,
                                                   const quint16 port,
                                                   const QString topic, QObject* parent)
    : MQTTSubscriber(host, port, topic, parent)
{


    connect(this, &MQTTPedestrianSubscriber::received, this, &MQTTPedestrianSubscriber::onReceived);
    connect(this, SIGNAL(sendAgents(Agents)), MainWindow::instance(), SLOT(receiveAgents(Agents)));

}

MQTTPedestrianSubscriber::~MQTTPedestrianSubscriber()
{

}

void MQTTPedestrianSubscriber::onReceived(const QMQTT::Message& message)
{

    _qout << "publish received: \"";// << QString::fromUtf8(message.payload())
    //<< "\"" << endl;

    QJsonObject obj;
    QJsonDocument doc = QJsonDocument::fromJson(message.payload());
    obj = doc.object();

    //qDebug() << obj;

    qint16 type=1;

    QJsonArray jsonArray = obj["pedestrians"].toArray();

    qDebug() << jsonArray.size();

    Agents agents_data;
    foreach (const QJsonValue & value, jsonArray) {
        QJsonObject p_obj = value.toObject();

        Agent a;
        a.id=p_obj["id"].toInt();
        QJsonObject tr_array = p_obj["translate"].toObject();
        a.setPosition(QVector3D(tr_array["x"].toDouble(), 0.0, tr_array["z"].toDouble()));
        QJsonObject v_array = p_obj["velocity"].toObject();
        a.setVelocity(QVector3D(v_array["x"].toDouble(), 0.0, v_array["z"].toDouble()));

        a.type=type;
        agents_data.push_back(a);
    }

    qDebug()<< "Sending pedestrian agents --> " << agents_data.size();

    sendAgents(agents_data);

}

