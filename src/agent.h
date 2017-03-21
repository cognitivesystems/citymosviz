#ifndef AGENT_H
#define AGENT_H

#include <QObject>
#include <QMetaType>
#include <QVector3D>
#include <iostream>

class Agent : public QObject
{
    Q_OBJECT

    Q_PROPERTY(qint16 id READ getId WRITE setId)
    Q_PROPERTY(QVector3D translate READ getPosition WRITE setPosition)
    Q_PROPERTY(QVector3D velocity READ getVelocity WRITE setVelocity)

public:
    Agent(QObject* parent = 0);
    ~Agent();

    Agent(const Agent& c){
        this->id=c.id;
        this->translate=c.translate;
        this->velocity=c.velocity;
    }

    int setId(const quint16 i){
        id=i;
    }

    int getId(){
        return id;
    }

    void setPosition( const QVector3D& p){
        translate=p;
    }

    QVector3D getPosition(){
        return translate;
    }

    void setVelocity( const QVector3D& v){
        velocity=v;
    }

    QVector3D getVelocity(){
        return velocity;
    }


public:
    quint16 id;
    QVector3D translate;
    QVector3D velocity;
};


#endif // AGENT_H