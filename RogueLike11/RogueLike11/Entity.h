#pragma once
#include "SuperObject.h"
#include "Point.h"
#include <iostream>
using namespace std;

class Entity : public SuperObject
{
public:
    int life = 1;

    Entity() : SuperObject() {}
    Entity(Point* placeP, char iconP, int lifeP, int speedP = 0, int directP = 0) :
        SuperObject(placeP, speedP, directP, iconP), life{ lifeP } {}
    virtual int collision_hanlder(SuperObject* obj)
    {
        if (typeid(obj) == typeid(Entity)) //EXAMPLE
        {
            obj = dynamic_cast<Entity*>(obj);
            life = 0;
        }
        return 1;
    }
};