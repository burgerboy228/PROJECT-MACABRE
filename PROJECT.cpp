#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <vector>

class Player {
    public:
    std::string name;
    char pchar;
    int health;
    int damage;
    int defense;
    int gold;
    int potion;
    int score;
    int xy[2];

    Player() : pchar('i'), health(100), damage(25), defense(40), gold(100), potion(1), score(0) {
        xy[0] = 1;
        xy[1] = 1;
    }
};

class PlayerBag {
    public:
    int potion;
    int swrdbst;
    int shldbst;

    PlayerBag() : potion(0), swrdbst(0), shldbst(0) {}
};

class Enemy {
    public:
    char echar;
    int health;
    int damage;
    int defense;
    int gold;
    int potion;
    int score;
    int xy[2];

    Enemy(char room[][20]) : echar('!'), health((rand() % 50) + 1), damage((rand() % 5) + 1), defense((rand() % 3) + 1), gold((rand() % 100) + 1), potion((rand() % 10) + 1), score((rand() % 100) + 1) {
        do {
            xy[0] = (rand() % 18) + 1;
            xy[1] = (rand() % 8) + 1;
        } while (room[xy[1]][xy[0]] == '#');
    }
};

class Chest {
    public:
    char cchar;
    int potion;
    int gold;
    int xy[2];
    bool active;

    Chest(char room[][20]) : cchar('C'), potion((rand() % 5) + 1), gold((rand() % 100) + 1), active(true) {
        do {
            xy[0] = (rand() % 18) + 1;
            xy[1] = (rand() % 8) + 1;
        } while (room[xy[1]][xy[0]] == '#');
    }
};

class Shop {
    public:
    const int items;
    const char schar;
    int potion[2];
    int shldbst[2];
    int swrdbst[2];
    int xy[2];

    Shop(char room[][20]) : items(3), schar('$') {
        potion[0] = (rand() % 5) + 1;
        potion[1] = 10;
        shldbst[0] = (rand() % 3) + 1;
        shldbst[1] = 20;
        swrdbst[0] = (rand() % 3) + 1;
        swrdbst[1] = 30;
        
        do {
            xy[0] = (rand() % 18) + 1;
            xy[1] = (rand() % 8) + 1;
        } while (room[xy[1]][xy[0]] == '#');
    }
};

void roomGen(char room[][20], int width, int height, Player &p1, Enemy &e1, Chest &c1, Shop &s1) {
    std::srand(std::time(0));

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (y == 0 || y == height - 1 || x == 0 || x == width - 1 ) {
                room[y][x] = '#';
            } else if ((y == p1.xy[1] && x == p1.xy[0]) || (y == e1.xy[1] && x == e1.xy[0]) || (y == c1.xy[1] && x == c1.xy[0]) || (y == s1.xy[1] && x == s1.xy[0])) {
                room[y][x] = ' ';
            } else {
                room[y][x] = (rand() % 4) ? ' ' : '#';
            }
        }
    }  
}               

void roomPrint(const char room[][20], const int width, const int height, const Player &p1, const Enemy &e1, const Chest &c1, const Shop &s1) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (y == p1.xy[1] && x == p1.xy[0]) { 
                std::cout << p1.pchar;
            } else if (y == e1.xy[1] && x == e1.xy[0]) {
                std::cout << e1.echar;   
            } else if (y == c1.xy[1] && x == c1.xy[0]) {
                std::cout << c1.cchar;
            } else if (y == s1.xy[1] && x == s1.xy[0]) {
                std::cout << s1.schar;
            } else {
                std::cout << room[y][x];
            }
        }
        std::cout << '\n';
    }
}

void playerInput(char uinput, char room[][20], const int width, int height, Player &p1) {
    switch(uinput) {
        case 'w':
            if (p1.xy[1] > 0 && room[p1.xy[1] - 1][p1.xy[0]] != '#') p1.xy[1]--;
            break;
        case 'a':
            if (p1.xy[0] > 0 && room[p1.xy[1]][p1.xy[0] - 1] != '#') p1.xy[0]--;
            break;
        case 's':
            if (p1.xy[1] < height - 1 && room[p1.xy[1] + 1][p1.xy[0]] != '#') p1.xy[1]++;
            break;
        case 'd':
            if (p1.xy[0] < width - 1 && room[p1.xy[1]][p1.xy[0] + 1] != '#') p1.xy[0]++;
            break;
        case 'q':
            p1.health -= 100;
            break;
        case 'b':
            std::cout << "####################################" << '\n';
            std::cout << "#             BAG MENU             #" << '\n';
            std::cout << "####################################" << '\n';
            std::cout << "# Gold: " << p1.gold << "                       #" << '\n';
            std::cout << "# Potion: " << p1.potion << "                     #" << '\n';
            std::cout << "####################################" << '\n';
            std::cout << "# 1: Use a potion                  #" << '\n';
            std::cout << "# 2: Go back                       #" << '\n';
            std::cout << "####################################" << '\n';
            std::cout << "> "; std::cin >> uinput; std::cout << '\n';
            break;
        default :
            std::cout << "INVALID INPUT: Please select a valid command (w/a/s/d/b)" << '\n';
            break;
    }
}

void shopInput(Shop &s1, Player &p1, PlayerBag &pb1) {
    int sIO;

    std::cout << "# SELECT ITEM OR GO BACK (1/2/3/4) #" << '\n'; 
    std::cout << "####################################" << '\n';
    std::cout << "> "; std::cin >> sIO; std::cout << "\n";

    switch(sIO) {
        case 1:
            if (s1.potion[0] > 0 && p1.gold >= s1.potion[1]) {
                pb1.potion += 1;
                s1.potion[0] -= 1;
                p1.gold -= s1.potion[1];
                std::cout << "Bought 1 Potion!" << '\n';
            } else {
                std::cout << "Cannot purchase item!" << '\n';
            }
            break;
        case 2:
            if (s1.shldbst[0] > 0 && p1.gold >= s1.shldbst[1]) {
                pb1.shldbst += 1;
                s1.shldbst[0] -= 1;
                p1.gold -= s1.shldbst[1];
                std::cout << "Bought 1 Shield Boost!" << '\n';
            } else {
                std::cout << "Cannot purchase item!" << '\n';
            }
            break;
        case 3:
            if (s1.swrdbst[0] > 0 && p1.gold >= s1.swrdbst[1]) {
                pb1.swrdbst += 1;
                s1.swrdbst[0] -= 1;
                p1.gold -= s1.swrdbst[1];
                std::cout << "Bought 1 Sword Boost!" << '\n';
            } else {
                std::cout << "Cannot purchase item!" << '\n';
            }
            break;
        case 4:
            break;
        default :
            std::cout << "Invalid Command: Enter a valid command (1/2/3/4)" << '\n';
            break;
    }
}

void enemyInput(Player &p1, Enemy &e1) {
    bool running = true;
    
    do {
        int run = (rand() % 2) + 1;
        int enemyattack = (e1.damage -= p1.defense);
        int playerattack = (p1.damage -= e1.defense);
        int playerdefend = (e1.damage - p1.defense) - 5;
        int uIO;

        std::cout << "> "; std::cin >> uIO; std::cout << '\n';
        
        switch(uIO) {
            case 1:
                if (enemyattack > 0) {
                    if (playerattack > 0) {
                        p1.health -= enemyattack;
                        e1.health -= playerattack;
                        std::cout << "You did " << playerattack << " damage!" << '\n';
                        std::cout << "But the enemy did " << enemyattack << " damage back" << '\n';
                    } else {
                        p1.health -= enemyattack;
                        e1.defense -= (p1.damage - (e1.defense / 2));
                        std::cout << "Your attack was blocked!" << '\n';
                        std::cout << "the enemy did " << enemyattack << " damage" << '\n';
                    }
                } else if(playerattack > 0) {
                    e1.health -= playerattack;
                    std::cout << "You did " << playerattack << " damage!" << '\n';
                    std::cout << "But the enemy's attack was blocked" << '\n';
                } else {
                    p1.health -= enemyattack;
                    e1.defense -= (p1.damage - (e1.defense / 2));
                    std::cout << "Your attack was blocked!" << '\n';
                    std::cout << "but the enemy's attack was blocked" << '\n';
                }
                break;
            case 2:
                if (playerdefend <= 0) {
                    std::cout << "Defended Succesfully" << '\n';
                } else {
                    p1.health -= playerdefend;
                    e1.defense -= (p1.damage - (e1.defense / 2));
                    std::cout << "The enemy did " << playerdefend << " damage!" << '\n';
                }
                break;
            case 3:
            // fix this shit or YOUR FIRED on foenem grave
                std::cout << "not done" << '\n';
                break;
            case 4:
                if (run == 1) {
                    running = false;
                    std::cout << "ran away safely!" << '\n';
                    break;
                } else {
                    p1.health -= enemyattack;
                    std::cout << "couldn't run away! The enemy did " << enemyattack << " damage!"  << '\n';
                    break;
                }
            default:
                std::cout << "INVALID: Invalid input, select (1/4)" << '\n';
                break;
        }
    } while (p1.health > 0 && e1.health > 0 && running);
}

void enemyReset(Enemy &e1, char room[][20]) {
    std::srand(std::time(0));
    
    e1.health = (rand() % 50) + 1; 
    e1.damage = (rand() % 5) + 1; 
    e1.defense = (rand() % 5) + 1; 
    e1.gold = (rand() % 100) + 1; 
    e1.potion = (rand() % 10) + 1; 
    e1.score = (rand() % 100) + 1;
    
    do {
        e1.xy[0] = (rand() % 18) + 1;
        e1.xy[1] = (rand() % 8) + 1;
    } while (room[e1.xy[1]][e1.xy[0]] == '#');
}

void printFightMenu() {
    std::cout << "####################################" << '\n';
    std::cout << "#             FIGHT MENU           #" << '\n';
    std::cout << "####################################" << '\n';
    std::cout << "# 1: Attack                        #" << '\n';
    std::cout << "# 2: Defend                        #" << '\n';
    std::cout << "# 3: Bag                           #" << '\n';
    std::cout << "# 4: Run                           #" << '\n';
    std::cout << "####################################" << '\n';
}

void printStartMenu() {
    std::cout << "4/8/25 @ 9:17 AM INDEV 1 KNOWN ERROR" << '\n';
    std::cout << "####################################" << '\n';
    std::cout << "#     WELCOME TO PROJECT SIGMA     #" << '\n';
    std::cout << "####################################" << '\n';
    std::cout << "#            how to play           #" << '\n';
    std::cout << "# wasd to move                     #" << '\n';
    std::cout << "# b for bag                        #" << '\n';
    std::cout << "# get the chest                    #" << '\n';
    std::cout << "# kill the enemy                   #" << '\n';
    std::cout << "####################################" << '\n';
    std::cout << "# ENTER YOUR NAME TO START         #" << '\n';
    std::cout << "####################################" << '\n';
}

void printShopMenu(const Shop &s1, const Player &p1) {
    std::cout << "####################################" << '\n';
    std::cout << "#               SHOP               #" << '\n';
    std::cout << "####################################" << '\n';
    std::cout << "# Potions: " << s1.potion[0] << " Cost: $" << s1.potion[1] << " #" << "\n";
    std::cout << "# Shield Boost: " << s1.shldbst[0] << " Cost: $" << s1.shldbst[1] << " #" << "\n";
    std::cout << "# Sword Boost: " << s1.swrdbst[0] << " Cost: $" << s1.swrdbst[1] << " #" << "\n";
    std::cout << "####################################" << '\n';
    std::cout << "# Your Gold: " << p1.gold << " #" << '\n';
    std::cout << "####################################" << '\n';
}

void slowPrint(const std::string& msg) {
    for (char c : msg) {
        std::cout << c << std::flush; 
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    std::cout << '\n';
}

bool checkCollision(const int a[2], const int b[2]) {
    return (a[0] == b[0] && a[1] == b[1]);
}

int main() {
    std::srand(std::time(0));

    const int width = 20; 
    const int height = 10;
    char room[height][width];
    char uinput;
    
    Player p1;
    PlayerBag pb1;
    Enemy e1(room);
    Chest c1(room);
    Shop s1(room);
    
    roomGen(room, width, height, p1, e1, c1, s1);

    printStartMenu();
    std::cout << ">"; std::cin >> p1.name; std::cout << '\n';
    
    do {
        std::cout << "Your Health: " << p1.health << '\n'; 
        std::cout << "Your Damage: " << p1.damage << '\n'; 
        std::cout << "Your Defence: " << p1.defense << '\n'; 
        std::cout << "Your Gold: " << p1.gold << '\n';
        roomPrint(room, width, height, p1, e1, c1, s1);
        std::cout << "> "; std::cin >> uinput; std::cout << "\n";
        playerInput(uinput, room, width, height, p1);
        if (checkCollision(p1.xy, e1.xy)) {
            printFightMenu();
            enemyInput(p1,e1);
        } else if (checkCollision(p1.xy, c1.xy)){
            if (c1.active) {
                p1.gold += c1.gold;
                p1.potion += c1.potion;
                c1.active = false;
                c1.cchar = 'x';
                std::cout << "You got " << c1.gold << " gold and " << c1.potion << " potion(s)!" << '\n';
            }
        } else if (checkCollision(p1.xy, s1.xy)){
            printShopMenu(s1, p1);
            shopInput(s1, p1, pb1);
        } else if (p1.health <= 0) {
            std::cout << "Well... " << p1.name << "It seems to have come to an end. You scored " << p1.score << "points" << '\n';
            break;
        } else if (e1.health <= 0) {
            std::cout << "You have killed the enemy" << '\n';
            std::cout << "You gained " << e1.score << " score!" << '\n';
            std::cout << "You gained " << e1.gold << " gold!" << '\n';
            p1.score += e1.score;
            p1.gold += e1.gold;
            roomGen(room, width, height, p1, e1, c1, s1);
            enemyReset(e1, room);
        }
    } while (p1.health > 0 && e1.health > 0);

    return 0;
}