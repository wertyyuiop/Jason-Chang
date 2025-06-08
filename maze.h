// maze.h  �]������^
#ifndef MAZE_H
#define MAZE_H

#include "block.h"
#include "portal.h"
#include "movable_goal.h"
#include <vector>
#include <string>
class Player;

class Maze {
private:
    std::vector<std::vector<Block*>> maze;
    std::vector<std::vector<char>>   pixel;
    vector<std::vector<bool>>    seen;
    int nRow=0, nCol=0;
    int gaming = 1;
    MovableGoal* movableGoal=nullptr;
    std::vector<Portal*> portals;

    // ���a��?�]�~��?�J��?�� new�^
    Player* playerPtr_ = nullptr;
    // �� playerPtr_ �O�_?��?�H?��??�w�R��? delete �O�_
    bool ownsPlayer_ = false;

    void initFromFile(const std::string& fn, int key_count);

public:
    int keyCount;
    // �u�n��?�@?�۳y�A�N?�F�G�ĤT???�ʬ٥i?
    // �p�G externalPlayer ? nullptr�A�N?�� new Player�F�_?�`�Υ��C
    Maze(const std::string& fn,
         int key_count = 0,
         Player* externalPlayer = nullptr);

    ~Maze();

    int get_nRow() const { return nRow; }
    int get_nCol() const { return nCol; }
    auto& get_maze()   { return maze; }
    auto& get_portals(){ return portals; }

    void readMaze();
    void printMaze(Player *p);
    void printStatus(Player *p);
    void start();
};

#endif // MAZE_H
