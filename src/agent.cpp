#include "agent.h"

Agent::Agent(QObject* parent){


    translate=new Vector3D();
    velocity=new Vector3D();

}

Agent::~Agent(){

    delete translate;
    delete velocity;
}
