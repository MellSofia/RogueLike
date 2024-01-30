#pragma once
#include <iostream>
using namespace std;

class Coord
{
public:
    int x = 0;
    int y = 0;
    Coord(int xP, int yP) :
        x{ xP }, y{ yP } {};
    Coord& operator()(int xP, int yP)
    {
        x = xP; y = yP;
        return *this;
    };
};