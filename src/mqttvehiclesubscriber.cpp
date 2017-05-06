#include "mqttvehiclesubscriber.h"
#include "mainwindow.h"

MQTTVehicleSubscriber::MQTTVehicleSubscriber(const QHostAddress& host,
                                             const quint16 port,
                                             const QString topic, QObject* parent)
    : MQTTSubscriber(host, port, topic, parent)
{

    connect(this, &MQTTVehicleSubscriber::received, this, &MQTTVehicleSubscriber::onReceived);
    connect(this, SIGNAL(sendAgents(Agents)), MainWindow::instance(), SLOT(receiveAgents(Agents)));

}

MQTTVehicleSubscriber::~MQTTVehicleSubscriber()
{

}

void MQTTVehicleSubscriber::onReceived(const QMQTT::Message& message)
{

    //_qout << "publish received: \"";
    QJsonObject obj;
    QJsonDocument doc = QJsonDocument::fromJson(message.payload());
    obj = doc.object();

    QJsonArray jsonArray = obj["vehicles"].toArray();

    qint16 type=0;

    static quint16 id_to_record;
    static bool init_done=false;
    static int counter =0;

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
        //        std::cout << a.translate.x() << " " << a.translate.y() << " " << a.translate.z() << " " << std::endl;

        if(!init_done){
            id_to_record=366;
            init_done=true;
        }
        else{
            if(a.id==id_to_record){
                //                std::cout << counter++ << " " << a.translate.x() << " " << a.translate.z() << std::endl;// << " " << a.translate.y() << std::endl;
            }
        }
    }

    qDebug()<< "Sending vehicle --> " << agents_data.size();

    sendAgents(agents_data);

}
