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
        
        place = p;
        p->into = this;
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
//char emptyChar = '.';

int fps = 4;
int latency = 1000 / fps;

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

class Case : public SuperObject 
{
public:
    vector<Item*> inventory;
    Case(Point* placeP, char iconP, int sizeP) : SuperObject(placeP, iconP), inventory(sizeP) {};
};

class Entity : public SuperObject
{
public:
    int life = 1;
    vector<Item*> inventory;
    Entity() : SuperObject(), inventory(1) {}
    Entity(Point* placeP, char iconP, int lifeP, int sizeP) :
        SuperObject(placeP, iconP), life{ lifeP }, inventory(sizeP) {}
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
    Human(Point* placeP, char iconP, int lifeP, int sizeP) :
        Entity(placeP, iconP, lifeP, sizeP) {}
    
};
class Monster : public Entity
{
public:
    bool friendly;
    Monster() : Entity(), friendly() {}
    Monster(Point* placeP, char iconP, int lifeP, int sizeP, bool friendlyP) :
        Entity(placeP, iconP, lifeP, sizeP), friendly{ friendlyP } {
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
    Human player;
    player.link(&display[5][5]);
    player.icon = '@';
    player.life = 10;
    Monster enemy(&display[5][7], '$', 15, 2, false);
    Instruments sword(&display[3][3], '!', 2, 3,20,1);
    Case casee(&display[2][4], 'o', 5);

    //добавление объектов в список
    objects.push_back(&player);
    objects.push_back(&enemy);
    objects.push_back(&sword);
    objects.push_back(&casee);

    Coord tempCoord(0, 0);
    int i = 0;
    while (main_flag)
    {
        // -----------STEP 1: input-----------
        // keyboard reciver
        keyboardPress = _getch();
        switch (keyboardPress)
        {
        case 'w':
            player.ismov = true;
            player.direct = 1;
            break;
        case 'd':
            player.ismov = true;
            player.direct = 2;
            break;
        case 's':
            player.ismov = true;
            player.direct = 3;
            break;
        case 'a':
            player.ismov = true;
            player.direct = 4;
            break;
        case ' ':
            break;
        case 27:
            main_flag = false;  //exit
            break;
        }

        // environment motor
        // здесь будет логика всех объектов: 
        // исполнение каких то паттернов движения, появление, применение свойств итд
        // в общем все, что должно произойти за этот такт
        if (i > 3) i = 0;
        enemy.ismov = true;
        enemy.direct = enemyMoved[i];
        ++i;
        // ---------STEP 2: processing---------
        // здесь же примененные действия обрабатываются, в частности - в блоке коллизии

        for (SuperObject* curObj : objects)
        {

            if (curObj->ismov)
            {
                tempCoord = curObj->move();
                if ((tempCoord.y >= 0 && tempCoord.y <= HIGH-1) and (tempCoord.x >= 0 && tempCoord.x <= WIDTH-1)) {
                    //проверка на то, есть ли что-то в этой точке (into=nulptr - false - пустота)
                    if (display[tempCoord.y][tempCoord.x].into and typeid(display[tempCoord.y][tempCoord.x].into) != typeid(Case))

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

        // -----------STEP 3: output-----------
        // вывод сцены на экран
        // очистка сцены и наполнение ее
        system("cls");
        displayFill();
        // добавление всех объектов на сцену
        // вывод сцены на экран
        
        displayOut();

        /*for (int i = 0; i < HIGH; ++i)
        {
            for (int j = 0; j < WIDTH; ++j)
            {
                cout << display[i][j].into << "  ";
            };
            cout << "\n";
        }*/

        cout << keyboardPress << endl;
        //Sleep(latency);
    }
}


//#include <iostream>
//#include <Windows.h>
//#include <vector>
//#include <string>
//#include <conio.h> // Для использования _getch()
//#include <list>
//using namespace std;
//class SuperObject
//{
//public:
//    char icon;
//    int x;
//    int y;
//    int speed = 0;
//    int direct = 0;
//    SuperObject(int xP, int yP, const char iconP, int speedP) : x{ xP }, y{ yP }, icon{ iconP }, speed{ speedP } {}
//    SuperObject() : SuperObject(30, 30, '.', 1) {}
//    virtual void collition(SuperObject& obj, char movement) {}
//    bool operator==(const SuperObject& other) const {
//        return x == other.x && y == other.y && icon == other.icon && speed == other.speed;
//    }
//};
//const int HIGHT = 10;
//const int WIDGH = 10;
//
//int fps = 25;
//int latency = 1000 / fps;
//char display[HIGHT][WIDGH];
//vector<SuperObject*> SuperObjectArr = {};
//int t = 0;
//void convert() {
//    for (int i = 0; i < HIGHT; i++) {
//        for (int j = 0; j < WIDGH; j++) {
//            display[i][j] = emptyChar;
//        }
//    }
//    for (int i = 0; i < SuperObjectArr.size(); i++) {
//        display[SuperObjectArr[i]->y][SuperObjectArr[i]->x] = SuperObjectArr[i]->icon;
//    }
//}
//void print() {
//    for (int i = 0; i < HIGHT; i++) {
//        for (int j = 0; j < WIDGH; j++) {
//            cout << display[i][j];
//        }
//        cout << "\n";
//    }
//}
//void What_to_do(char movement) {
//    int newX = SuperObjectArr[0]->x;
//    int newY = SuperObjectArr[0]->y;
//
//    switch (movement) {
//    case ('w'):
//        newY = SuperObjectArr[0]->y - 1;
//        break;
//    case ('s'):
//        newY = SuperObjectArr[0]->y + 1;
//        break;
//    case ('a'):
//        newX = SuperObjectArr[0]->x - 1;
//        break;
//    case ('d'):
//        newX = SuperObjectArr[0]->x + 1;
//        break;
//    default:
//        //cout << "Не двигается";
//        return;
//    }
//
//    if (newX >= 0 && newX < WIDGH && newY >= 0 && newY < HIGHT && display[newY][newX] == emptyChar) {
//        SuperObjectArr[0]->x = newX;
//        SuperObjectArr[0]->y = newY;
//    }
//}
//
//void tolk(char movement) {
//    int newX = SuperObjectArr[0]->x;
//    int newY = SuperObjectArr[0]->y;
//    int newX1 = SuperObjectArr[1]->x;
//    int newY1 = SuperObjectArr[1]->y;
//    int newX2 = SuperObjectArr[2]->x;
//    int newY2 = SuperObjectArr[2]->y;
//    switch (movement) {
//    case ('w'):
//        if ((newX == newX1) && (newY - 1 == newY1)) {
//            newY1 = SuperObjectArr[1]->y - 1;
//            newY = SuperObjectArr[0]->y - 1;
//        }
//        else {
//            newY = SuperObjectArr[0]->y - 1;
//        }
//        break;
//    case ('s'):
//        if ((newX == newX1) && (newY + 1 == newY1)) {
//            newY1 = SuperObjectArr[1]->y + 1;
//            newY = SuperObjectArr[0]->y + 1;
//        }
//        else {
//            newY = SuperObjectArr[0]->y + 1;
//        }
//        break;
//    case ('a'):
//        if ((newX - 1 == newX1) && (newY == newY1)) {
//            newX = SuperObjectArr[0]->x - 1;
//            newX1 = SuperObjectArr[1]->x - 1;
//        }
//        else {
//            newX = SuperObjectArr[0]->x - 1;
//        }
//        break;
//    case ('d'):
//        if ((newX + 1 == newX1) && (newY == newY1)) {
//            newX = SuperObjectArr[0]->x + 1;
//            newX1 = SuperObjectArr[1]->x + 1;
//        }
//        else {
//            newX = SuperObjectArr[0]->x + 1;
//        }
//        break;
//    default:
//        cout << "Не двигается";
//        return;
//    }
//    if ((newX >= 0 && newX < WIDGH && newY >= 0 && newY < HIGHT && display[newY][newX] == emptyChar) || (newX1 >= 0 && newX1 < WIDGH && newY1 >= 0 && newY1 < HIGHT && display[newY1][newX1] == emptyChar)) {
//        SuperObjectArr[1]->x = newX1;
//        SuperObjectArr[1]->y = newY1;
//        SuperObjectArr[0]->x = newX;
//        SuperObjectArr[0]->y = newY;
//    }
//}
//
//class Entity :public SuperObject {
//private:
//public:
//    int life;
//    Entity(int xP, int yP, const char iconP, int speedP, int nlife) : SuperObject(xP, yP, iconP, speedP), life(nlife) {}
//    void loss_life(int nlife, int t) {
//        if ((life > 0) && (t == 0)) {
//            life -= nlife;
//        }
//        if ((life == 0) && (t == 0)) {
//            auto it = find_if(SuperObjectArr.begin(), SuperObjectArr.end(), [this](SuperObject* obj) {
//                return obj == this;
//                });
//            if ((it != SuperObjectArr.end()) && (t == 0)) {
//                SuperObjectArr.erase(it);
//                t = 1;
//            }
//            cout << t << endl;
//        }
//    }
//
//    void add_life(int nlife) {
//        life += nlife;
//    }
//    void collition(SuperObject& obj, char movement) {
//        if (typeid(obj) == typeid(SuperObject)) {
//            tolk(movement);
//        }
//        if (typeid(obj) == typeid(Entity)) {
//            int newX0 = SuperObjectArr[0]->x;
//            int newX2 = SuperObjectArr[2]->x;
//            int newY0 = SuperObjectArr[0]->y;
//            int newY2 = SuperObjectArr[2]->y;
//            if (abs(newX0 - newX2) + abs(newY0 - newY2) == 1) {
//                hit(dynamic_cast<Entity&>(obj), movement);
//                cout << "ddddddddddddddddddd";
//            }
//        }
//    }
//    void hit(Entity& monster, char movement) {
//        int t = 0;
//        monster.loss_life(1, t);
//        cout << t;
//    }
//    ~Entity() {
//    }
//};
//int main()
//{
//    setlocale(LC_ALL, "rus");
//    Sleep(1000);
//    system("cls");
//    Entity me{ 4, 5, '@', 1, 20 };
//    SuperObjectArr.push_back(&me);
//    SuperObject box{ 4, 2, '&', 1 };
//    SuperObjectArr.push_back(&box);
//    Entity monster{ 5, 5, '$', 1, 5 };
//    SuperObjectArr.push_back(&monster);
//    bool main_flag = true;
//    char movement = {};
//    list <Box<SuperObject*>> l;
//
//    while (main_flag)
//    {
//        convert();
//        print();
//        if (_kbhit()) {
//            movement = _getch();
//            me.collition(box, movement);
//            if ((movement == 'e') && (monster.life >= 0)) {
//                me.collition(monster, movement);
//            }
//        }
//        Sleep(latency);
//        system("cls");
//    }
//}