#pragma once


class Item : public SuperObject
{
public:
    int temp = 2;

    Item() : SuperObject() {}
    Item(Point* placeP, char iconP, int tempP, int speedP = 0, int directP = 0) :
        SuperObject(placeP, speedP, directP, iconP), temp{ tempP } {}

    virtual int collision_hanlder(SuperObject* obj)
    {
        //your code here
        return 1;
    }
};