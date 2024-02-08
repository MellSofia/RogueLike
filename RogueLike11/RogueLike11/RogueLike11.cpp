#include <iostream>
#include <windows.h>
#include <list>
#include <conio.h>
#include <vector>
using namespace std;
char emptyChar = '.';

class SuperObject;
class Coord
{
public:
    int x{ 0 };
    int y{ 0 };
    Coord(int xP, int yP) :
        x{ xP }, y{ yP } {};
    Coord& operator()(int xP, int yP)
    {
        x = xP; y = yP;
        return *this;
    };
};
class Point
{
public:
    char icon{ emptyChar };
    Coord coord;
    SuperObject* into{ nullptr };
    Point() : icon{ emptyChar }, coord(0, 0) {};
    Point(Coord coordP) : coord{ coordP } {};
    Point(Coord coordP, SuperObject* intoP, char iconP = emptyChar) :
        icon{ iconP }, coord(coordP), into{ intoP } {};

    Point* operator()(char iconP)
    {
        icon = iconP;
        return this;
    }

    void clear(int xP = 0, int yP = 0)
    {
        coord(0, 0);
        icon = emptyChar;
        into = nullptr;

    }
};
ostream& operator<<(ostream& out, Point& obj)
{
    return out << obj.icon;
}

class SuperObject
{
public:
    Point* place;
    char icon;
    int speed{ 1 }; 
    bool ismov{ false }; 
    int direct{ 0 };
    SuperObject() :
        place{ nullptr }, icon{ emptyChar } {}
    SuperObject(Point* placeP, char iconP = emptyChar, int speedP = 1, bool ismovP = false, int directP = 0) :
        speed{ speedP }, direct{ directP }, ismov{ ismovP }, icon{ iconP }
    {
        link(placeP);
    }


    virtual Coord* getCoord()
    {
        return &place->coord;
    }

    virtual void link(Point* p)
    {
        if (place != nullptr) place->into = nullptr;
        if (p->into != nullptr) {}
        else{
            place = p;
            p->into = this;
        }
    }
    virtual int collision_hanlder(SuperObject* obj)
    {
        return 0;
    };
    virtual Coord move()
    {
        Coord tcoord(place->coord);
        tcoord=place->coord;
        switch (direct)
        {
        case 0:
            break;
        case 1:
            tcoord.y -= speed; break;
        case 2:
            tcoord.x += speed; break;
        case 3:
            tcoord.y += speed; break;
        case 4:
            tcoord.x -= speed; break;
        default:
            break;
        }
        return tcoord;
    }
};

//----- env var -----

const int HIGH = 15;
const int WIDTH = 10;
bool openedy = false;

int fps = 4;
int latency = 1000 / fps;
//Item opening[20];

Point display[HIGH][WIDTH]{};
list<SuperObject*> objects;

char keyboardPress;
bool main_flag = true;
//===== env var =====
void displayClearField()
{
    for (int i = 0; i < HIGH; ++i)
    {
        for (int j = 0; j < WIDTH; ++j)
        {
            display[i][j].clear(j, i);
        };
    }
}
void displayFill()
{
    for (int i = 0; i < HIGH; ++i)
    {
        for (int j = 0; j < WIDTH; ++j)
        {
            if (!display[i][j].into) display[i][j].icon = emptyChar;
            else { display[i][j].icon = display[i][j].into->icon; }
        };
    }
}
void displayOut()
{
    for (int i = 0; i < HIGH; ++i)
    {
        for (int j = 0; j < WIDTH; ++j)
        {
            cout << display[i][j];
        };
        cout << "\n";
    }
}
class Item : public SuperObject
{
public:
    int temp = 2;

    Item() : SuperObject() {}
    Item(Point* placeP, char iconP, int tempP) :
        SuperObject(placeP, iconP), temp{ tempP } {}

    
};

//class Player;
class Case : public SuperObject 
{
public:
    vector<Item*> inventory;
    bool opened{ false };
    Case(Point* placeP, char iconP, int sizeP) : SuperObject(placeP, iconP), inventory(sizeP) {};
    virtual int collision_hanlder(SuperObject* obj) {
        /*if (typeid(*obj) == typeid(Player)) {
            openedy=false;
        }*/
        return 1;
    }

};

class Entity : public SuperObject
{
public:
    int life = 1;
    vector<Item*> inventory;
    Entity() : SuperObject(), inventory(1) {}
    Entity(Point* placeP, char iconP, int lifeP, int speedP, int directP, int sizeP) :
        SuperObject(placeP, iconP, speedP, directP), life{ lifeP }, inventory(sizeP) {}
    virtual int collision_hanlder(SuperObject* obj)
    {
        if (typeid(obj) == typeid(Case)) //EXAMPLE
        {
            obj = dynamic_cast<Case*>(obj);
            link(obj->place);
            ismov = false;
        }
        return 1;
    }
};

class Human : public Entity 
{
public:
    Human() : Entity() { }
    Human(Point* placeP, char iconP, int lifeP, int speedP, int directP, int sizeP) :
        Entity(placeP, iconP, speedP, directP, lifeP, sizeP) {}
    virtual int collition_hendler(SuperObject* obj) {
        if (typeid(obj) == typeid(Case)) {
            
            return 1;
        }
    }
};

int nn = 0;
class Player : public Entity
{
public:
    Player() : Entity() { }
    Player(Point* placeP, char iconP, int lifeP, int sizeP, int speedP=1, int directP=0) :
        Entity(placeP, iconP, speedP, directP, lifeP, sizeP){}
    void displayInventory(Case* caseobj) {
        for (int i = 0; i < caseobj->inventory.size(); i++) {
            if (caseobj->inventory[i] != nullptr)
                cout << "Inventory:" << caseobj->inventory[i]->icon << "|";
        }
    }
    virtual int collision_hanlder(SuperObject* obj) {
        //nn = 1;
        //char save;
        if (typeid(*obj) == typeid(Case)) {
            //nn = 2;
            //save = obj->icon;
            //cout << save;
            Case *caseobj = dynamic_cast<Case*>(obj);
            if (caseobj) {
                link(caseobj->place);
                link(this->place);
                //nn = 3;
                openedy = true;
                caseobj->icon = 'N';
            }
        }
        return 1;
    }
};
class Monster : public Entity
{
public:
    bool friendly;
    Monster() : Entity(), friendly() {}
    Monster(Point* placeP, char iconP, int lifeP, int sizeP, bool friendlyP, int speedP=1, int directP=1) :
        Entity(placeP, iconP, speedP, directP, lifeP, sizeP), friendly{ friendlyP } {
    }
};
class Instruments :public Item
{
public:
    int damage;
    int life;
    int ammo;
    Instruments() : Item(), damage(1), life(1), ammo(1){}
    Instruments(Point* placeP, char iconP, int tempP, int damageP, int lifeP, int ammoP) :
        Item(placeP, iconP, tempP), damage{ damageP }, life{ lifeP }, ammo{ ammoP } {}
};
class Coin :public Item
{
public:
    int volume;
    Coin() : Item(),  volume(0) {}
    Coin(Point* placeP, char iconP, int tempP, int volumeP) :
        Item(placeP, iconP, tempP), volume{volumeP} {}
};
int enemyMoved[4]{1,2,3,4};
char animates[5]{ '.', ',', ';', '|', '\'' };
int main()
{
    for (int i = 0; i < HIGH; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            display[i][j].coord(j, i);
        }
    };
    Player player;
    char side[4] = { 30,16,31,17 };
    player.link(&display[5][5]);
    player.icon = side[player.direct];
    player.life = 10;
    Monster enemy(&display[5][7], '$', 15, 2, false);
    Instruments sword(&display[3][3], '!', 2, 3,20,1);
    Case casee(&display[2][4], 'O', 5);
    using namespace std;
    casee.inventory.push_back(&sword);
    objects.push_back(&player);
    objects.push_back(&enemy);
    objects.push_back(&sword);
    objects.push_back(&casee);

    Coord tempCoord(0, 0);
    int i = 0;
    while (main_flag)
    {

        keyboardPress = _getch();
        system("cls");

        switch (keyboardPress)
        {
        case 'w':
            player.ismov = true;
            player.direct = 1;
            player.icon = side[0];
            break;
        case 'd':
            player.ismov = true;
            player.direct = 2;
            player.icon = side[1];
            break;
        case 's':
            player.ismov = true;
            player.direct = 3;
            player.icon = side[2];
            break;
        case 'a':
            player.ismov = true;
            player.direct = 4;
            player.icon = side[3];
            break;
        case ' ':
            break;
        case 27:
            main_flag = false; 
            break;
        }

        if (i > 3) i = 0;
        enemy.ismov = true;
        enemy.direct = enemyMoved[i];
        ++i;

        for (SuperObject* curObj : objects)
        {

            if (curObj->ismov)
            {
                tempCoord = curObj->move();
                if ((tempCoord.y >= 0 && tempCoord.y <= HIGH-1) and (tempCoord.x >= 0 && tempCoord.x <= WIDTH-1)) {
                     if (display[tempCoord.y][tempCoord.x].into )

                    {
                        display[tempCoord.y][tempCoord.x].into->collision_hanlder(curObj);
                        curObj->collision_hanlder(display[tempCoord.y][tempCoord.x].into);
                    }
                    else
                    {
                        curObj->link(&display[tempCoord.y][tempCoord.x]);
                        curObj->ismov = false;
                    }
                }
            }
        }
        displayFill();
        displayOut();
        cout << "life: " << player.life <<endl;
        //cout << nn;
        //cout << endl;
        while (openedy) {
            player.displayInventory(&casee);
            openedy = false;
        }
        cout << endl;

        cout << keyboardPress << endl;
    }
}