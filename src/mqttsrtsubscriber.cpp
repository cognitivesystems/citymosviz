#include "mqttsrtsubscriber.h"
#include "mainwindow.h"

MQTTSRTSubscriber::MQTTSRTSubscriber(const QHostAddress& host,
                                             const quint16 port,
                                             const QString topic, QObject* parent)
    : MQTTSubscriber(host, port, topic, parent){

    connect(this, &MQTTVehicleSubscriber::received, this, &MQTTSRTSubscriber::onReceived);
    connect(this, SIGNAL(sendAgents(Agents)), MainWindow::instance(), SLOT(receiveAgents(Agents)));

}

MQTTSRTSubscriber::~MQTTSRTSubscriber(){

}

void MQTTSRTSubscriber::onReceived(const QMQTT::Message& message){
    //_qout << "publish received: \"";
    QJsonObject obj;
    QJsonDocument doc = QJsonDocument::fromJson(message.payload());
    obj = doc.object();

    QJsonArray jsonArray = obj["vehicles"].toArray();

    qint16 type=2;

    Agents agents_data;
    foreach (const QJsonValue & value, jsonArray) {
        QJsonObject p_obj = value.toObject();

        Agent a;
        a.id=p_obj["id"].toString().toInt();
        QJsonObject tr_array = p_obj["location"].toObject();
        a.setPosition(QVector3D(tr_array["x"].toString().toDouble(), 0.0, tr_array["y"].toString().toDouble()));
        QJsonObject v_array = p_obj["velocity"].toObject();
        a.setVelocity(QVector3D(v_array["x"].toString().toDouble(), 0.0, v_array["y"].toString().toDouble()));

        a.type=type;
        agents_data.push_back(a);
    }

    qDebug()<< "Sending vehicle --> " << agents_data.size();

    sendAgents(agents_data);
}

