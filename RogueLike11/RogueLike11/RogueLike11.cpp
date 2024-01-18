#include <iostream>
#include <Windows.h>
#include <vector>
#include <string>
#include <conio.h> // Для использования _getch()
using namespace std;

class SuperObject
{
public:
    char icon;
    int x;
    int y;
    int speed;
    SuperObject(int xP, int yP, const char iconP, int speedP) : x{ xP }, y{ yP }, icon{ iconP }, speed{ speedP } {}
    SuperObject() : SuperObject(30, 30, '.', 1) {}
    virtual void collition(SuperObject& obj, char movement) {}
    bool operator==(const SuperObject& other) const {
        return x == other.x && y == other.y && icon == other.icon && speed == other.speed;
    }
};

const int HIGHT = 10;
const int WIDGH = 10;

int fps = 25;
int latency = 1000 / fps;
char display[HIGHT][WIDGH];
vector<SuperObject*> SuperObjectArr = {};
char iconDis = '.';
int t = 0;
void convert() {
    for (int i = 0; i < HIGHT; i++) {
        for (int j = 0; j < WIDGH; j++) {
            display[i][j] = iconDis;
        }
    }
    for (int i = 0; i < SuperObjectArr.size(); i++) {
        display[SuperObjectArr[i]->y][SuperObjectArr[i]->x] = SuperObjectArr[i]->icon;
    }
}

void print() {
    for (int i = 0; i < HIGHT; i++) {
        for (int j = 0; j < WIDGH; j++) {
            cout << display[i][j];
        }
        cout << "\n";
    }
}

void What_to_do(char movement) {
    int newX = SuperObjectArr[0]->x;
    int newY = SuperObjectArr[0]->y;

    switch (movement) {
    case ('w'):
        newY = SuperObjectArr[0]->y - 1;
        break;
    case ('s'):
        newY = SuperObjectArr[0]->y + 1;
        break;
    case ('a'):
        newX = SuperObjectArr[0]->x - 1;
        break;
    case ('d'):
        newX = SuperObjectArr[0]->x + 1;
        break;
    default:
        //cout << "Не двигается";
        return;
    }

    if (newX >= 0 && newX < WIDGH && newY >= 0 && newY < HIGHT && display[newY][newX] == iconDis) {
        SuperObjectArr[0]->x = newX;
        SuperObjectArr[0]->y = newY;
    }
}

void tolk(char movement) {
    int newX = SuperObjectArr[0]->x;
    int newY = SuperObjectArr[0]->y;
    int newX1 = SuperObjectArr[1]->x;
    int newY1 = SuperObjectArr[1]->y;
    int newX2 = SuperObjectArr[2]->x;
    int newY2 = SuperObjectArr[2]->y;
    switch (movement) {
    case ('w'):
        if ((newX == newX1) && (newY - 1 == newY1)) {
            newY1 = SuperObjectArr[1]->y - 1;
            newY = SuperObjectArr[0]->y - 1;
        }
        else {
            newY = SuperObjectArr[0]->y - 1;
        }
        break;
    case ('s'):
        if ((newX == newX1) && (newY + 1 == newY1)) {
            newY1 = SuperObjectArr[1]->y + 1;
            newY = SuperObjectArr[0]->y + 1;
        }
        else {
            newY = SuperObjectArr[0]->y + 1;
        }
        break;
    case ('a'):
        if ((newX - 1 == newX1) && (newY == newY1)) {
            newX = SuperObjectArr[0]->x - 1;
            newX1 = SuperObjectArr[1]->x - 1;
        }
        else {
            newX = SuperObjectArr[0]->x - 1;
        }
        break;
    case ('d'):
        if ((newX + 1 == newX1) && (newY == newY1)) {
            newX = SuperObjectArr[0]->x + 1;
            newX1 = SuperObjectArr[1]->x + 1;
        }
        else {
            newX = SuperObjectArr[0]->x + 1;
        }
        break;
    default:
        cout << "Не двигается";
        return;
    }
    if ((newX >= 0 && newX < WIDGH && newY >= 0 && newY < HIGHT && display[newY][newX] == iconDis) || (newX1 >= 0 && newX1 < WIDGH && newY1 >= 0 && newY1 < HIGHT && display[newY1][newX1] == iconDis)) {
        SuperObjectArr[1]->x = newX1;
        SuperObjectArr[1]->y = newY1;
        SuperObjectArr[0]->x = newX;
        SuperObjectArr[0]->y = newY;
    }
}

class Entity :public SuperObject {
private:
public:
    int life;
    Entity(int xP, int yP, const char iconP, int speedP, int nlife) : SuperObject(xP, yP, iconP, speedP), life(nlife) {}
    void loss_life(int nlife, int t) {
        if ((life > 0) && (t == 0)) {
            life -= nlife;
        }
        if ((life == 0) && (t == 0)) {
            auto it = find_if(SuperObjectArr.begin(), SuperObjectArr.end(), [this](SuperObject* obj) {
                return obj == this;
                });
            if ((it != SuperObjectArr.end()) && (t == 0)) {
                SuperObjectArr.erase(it);
                t = 1;
            }
            cout << t << endl;
        }
    }

    void add_life(int nlife) {
        life += nlife;
    }
    void collition(SuperObject& obj, char movement) {
        if (typeid(obj) == typeid(SuperObject)) {
            tolk(movement);
        }
        if (typeid(obj) == typeid(Entity)) {
            int newX0 = SuperObjectArr[0]->x;
            int newX2 = SuperObjectArr[2]->x;
            int newY0 = SuperObjectArr[0]->y;
            int newY2 = SuperObjectArr[2]->y;
            if (abs(newX0 - newX2) + abs(newY0 - newY2) == 1) {
                hit(dynamic_cast<Entity&>(obj), movement);
                cout << "ddddddddddddddddddd";
            }
        }
    }
    void hit(Entity& monster, char movement) {
        int t = 0;
        monster.loss_life(1, t);
        cout << t;
    }
    ~Entity() {
    }
};
int main()
{
    setlocale(LC_ALL, "rus");
    Sleep(1000);
    system("cls");
    Entity me{ 4, 5, '@', 1, 20 };
    SuperObjectArr.push_back(&me);
    SuperObject box{ 4, 2, '&', 1 };
    SuperObjectArr.push_back(&box);
    Entity monster{ 5, 5, '$', 1, 5 };
    SuperObjectArr.push_back(&monster);
    bool main_flag = true;
    char movement = {};
    while (main_flag)
    {
        convert();
        print();
        if (_kbhit()) {
            movement = _getch();
            me.collition(box, movement);
            if ((movement == 'e') && (monster.life >= 0)) {
                me.collition(monster, movement);
            }
        }
        Sleep(latency);
        system("cls");
    }
}