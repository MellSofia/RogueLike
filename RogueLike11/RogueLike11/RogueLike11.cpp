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
        if (place != nullptr)
            place->into = nullptr;

        if (p->into != nullptr) {}
        else
        {
            place = p;
            p->into = this;
        }
    }
    virtual void delink()
    {
        if (place != nullptr)
        {
            place->into = nullptr;
            place = nullptr;
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
vector<SuperObject*> objects;

int enemyMoved[4]{ 1,2,3,4 };
char animates[5]{ '.', ',', ';', '|', '\'' };

char keyboardPress;
bool main_flag = true;
char heart = 3;
char cross = 197;
int i = 0;
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

    Item() : SuperObject() {}
    Item(Point* placeP, char iconP) :
        SuperObject(placeP, iconP){}
};

//class Player;
void deleting(SuperObject* obj) {
    for (int i = 0; i < objects.size(); i++) {
        if (objects[i] == obj) {
            display[objects[i]->getCoord()->x][objects[i]->getCoord()->y].into = nullptr;
            objects.erase(objects.cbegin() + i);
            break;
        }
    }
}

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

class Instruments :public Item
{
public:
    int damagePlus;
    int life;
    int ammo;
    Instruments() : Item(), damagePlus(1), life(1), ammo(1) {}
    Instruments(Point* placeP, char iconP, int damagePlusP, int lifeP, int ammoP) :
        Item(placeP, iconP), damagePlus{ damagePlusP }, life{ lifeP }, ammo{ ammoP } {}
};

class Coin :public Item
{
public:
    int volume;
    Coin() : Item(), volume(0) {}
    Coin(Point* placeP, char iconP, int volumeP) :
        Item(placeP, iconP), volume{ volumeP } {}
};

vector<Item*> temp_inventory;
class Entity : public SuperObject
{
public:
    int life = 1;
    int damage;
    vector<Item*> inventory;
    Entity() : SuperObject(), inventory(1), damage(1) {}
    Entity(Point* placeP, char iconP, int lifeP, int damageP, int speedP = 0, int directP = 0, int sizeP=1) :
        SuperObject(placeP, speedP, directP, iconP), life{ lifeP }, inventory(1), damage{ damageP } {}
    
    virtual int collision_hanlder(SuperObject* obj)
    {
        if (typeid(obj) == typeid(Case)) 
        {
            obj = dynamic_cast <Case*>(obj);
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
    Human(Point* placeP, char iconP, int lifeP, int damageP, int speedP = 0, int directP = 0, int sizeP = 1) :
        Entity(placeP, speedP,  directP, iconP, lifeP, damageP, sizeP) {}
    virtual int collition_hendler(SuperObject* obj) {
        if (typeid(obj) == typeid(Case)) {
            
            return 1;
        }
    }
};

class Monster : public Entity
{
public:
    bool friendly;
    Monster() : Entity(), friendly() {}
    Monster(Point* placeP, char iconP, int lifeP, int damageP, bool friendlyP, int speedP = 0, int directP = 0, int sizeP = 1) :
        Entity(placeP, speedP, directP, iconP, lifeP, damageP, sizeP), friendly{friendlyP} {}
    virtual int collision_hanlder(SuperObject* obj);
    
};
void circle_move(Monster& enemy) {
    //int i = 0;
    if (i > 3) i = 0;
    enemy.ismov = true;
    enemy.direct = enemyMoved[i];
    ++i;
}

bool see = false;
class Player : public Entity
{
public:
    Player() : Entity() { }
    Player(Point* placeP, char iconP, int lifeP, int damage, int sizeP=1, int speedP=1, int directP=0) :
        Entity(placeP, iconP, speedP, directP, lifeP, 1){}
    void displayInventory(Case* caseobj) {
        for (int i = 0; i < caseobj->inventory.size(); i++) {
            if (caseobj->inventory[i] != nullptr)
                cout << "Inventory:" << caseobj->inventory[i]->icon << "|";
        }
    }
    virtual int collision_hanlder(SuperObject* obj)
    {
        if (typeid(*obj) == typeid(Case))
        {
            Case* caseObj = dynamic_cast<Case*>(obj);
            if (caseObj)
            {
                caseObj->icon = 'G';
                link(caseObj->place);
                link(this->place);
            }
                for (int i = 0; i < caseObj->inventory.size(); i++) {
                    if (caseObj->inventory[i] != nullptr) temp_inventory.push_back(caseObj->inventory[i]);
                }
        }
        else if (typeid(*obj) == typeid(Instruments))
        {
            Instruments* instrumentObj = dynamic_cast<Instruments*>(obj);
        
            if (inventory[0] != nullptr) {
                delete inventory[0]; // Удаляем старый предмет
            }
            else {
                this->inventory[0] = instrumentObj;
                this->damage += instrumentObj->damagePlus;
                deleting(instrumentObj);
            }
        }
        else if (typeid(*obj) == typeid(Monster)) 
        {
            Monster* monsterObj = dynamic_cast<Monster*>(obj);
            if (monsterObj->life > 0) 
            {
                monsterObj->life -= this->damage;
                if (monsterObj->life <= 0)
                {
                    monsterObj->ismov = false;
                    deleting(monsterObj);
                    monsterObj->icon = 206;
                }
            }
            else {
                Point* tempP(monsterObj->place);
                monsterObj->delink();
                Coin coino(tempP,'@', 1);
                objects.push_back(&coino);
                                
                cout << endl;
            }
        }
    return 1;
    }
};
int Monster::collision_hanlder(SuperObject* obj) {
    if (typeid(*obj) == typeid(Player)) {
        Player* player = dynamic_cast<Player*>(obj);
        if (this->life > 0) {
            player->life -= this->damage;
        }
        
    }
    return 1;
}

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
    player.life = 15;
    player.damage = 1;
    Monster enemy1(&display[2][3],'$', 5, 1, false);
    enemy1.damage = 1;
    enemy1.life = 5;
    Monster enemy2(&display[5][7], '$', 5, 1, false);
    enemy2.damage = 1;
    enemy2.life = 5;
    Monster enemy3(&display[9][2], '$', 5, 1, false);
    enemy3.damage = 1;
    enemy3.life = 5;
    Instruments sword(&display[3][3], '!', 2, 3,20);
    Instruments sword2;
    enemy1.inventory.push_back(&sword2);
    Instruments sword3(&display[5][3], '#', 2, 3, 20);
    Case casee(&display[2][4], 'O', 5);
    casee.inventory.push_back(&sword);
    objects.push_back(&player);
    objects.push_back(&enemy1);
    objects.push_back(&enemy2);
    objects.push_back(&enemy3);
    objects.push_back(&sword);
    objects.push_back(&sword3);
    objects.push_back(&casee);

    Coord tempCoord(0, 0);

    while (main_flag)
    {

        keyboardPress = _getch();
        //system("cls");

        switch (keyboardPress)
        {
        case 'w' :
        case 'H' :
            player.ismov = true;
            player.direct = 1;
            player.icon = side[0];
            break;
        case 'd':
        case 'M':
            player.ismov = true;
            player.direct = 2;
            player.icon = side[1];
            break;
        case 's':
        case 'P':
            player.ismov = true;
            player.direct = 3;
            player.icon = side[2];
            break;
        case 'a':
        case 'K':
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
        circle_move(enemy1);
        circle_move(enemy2);
        circle_move(enemy3);
        for (int i = 0; i < objects.size(); i++)
        {
            SuperObject* curObj = objects[i];
            for (SuperObject* curObj : objects)
            {

                if (curObj->ismov)
                {
                    tempCoord = curObj->move();
                    if ((tempCoord.y >= 0 && tempCoord.y <= HIGH - 1) and (tempCoord.x >= 0 && tempCoord.x <= WIDTH - 1)) {
                        if (display[tempCoord.y][tempCoord.x].into)

                        {
                            //display[tempCoord.y][tempCoord.x].into->collision_hanlder(curObj);
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
        }
        system("cls");
        displayFill();
        displayOut();
        cout << "Life: ";
        for (int i = 1; i < player.life; i++) cout << heart;
        cout << endl;

        cout << "Damage: " << player.damage << endl;
        
        cout << "Inventory: [ ";
        for (int i = 0; i < player.inventory.size(); i++) {
            if (player.inventory[i] != nullptr) { cout << player.inventory[i]->icon << " "; }
            else  cout << "| ";
        }
        cout << "]";
        
        //cout << endl << cross << endl;
        cout << endl;
        cout << keyboardPress << endl;
        cout << enemy1.life << endl;
        cout << enemy2.life << endl;
        cout << enemy3.life << endl;

        if (player.life <= 0) {
            main_flag = false;
        }
    }
}