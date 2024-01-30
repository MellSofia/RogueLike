#pragma once

class SuperObject
{
    Point* place;
public:
    char icon;
    int speed;
    int direct;
    SuperObject() :
        place{ nullptr }, speed{ 0 }, direct{ 0 }, icon{ emptyChar } {}
    SuperObject(Point* placeP, int speedP = 0, int directP = 0, char iconP = emptyChar) :
        speed{ speedP }, direct{ directP }, icon{ iconP } {
        link(placeP);
    }

    virtual Coord& getCoord()
    {
        return place->coord;
    }
    virtual void link(Point* p)
    {
        p->into = nullptr;
        place = p;
        p->into = this;
    }
    virtual int collision_hanlder(SuperObject* obj)
    {
        return 0;
    };
    virtual Coord move() //возвращает координаты следующего местоположения. не меняет текущие параметры
    {
        Coord tcoord(place->coord);
        //your code here
        return tcoord;
    }
};