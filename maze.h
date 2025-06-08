// maze.h  （完整文件）
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

    // 玩家指?（外部?入或?部 new）
    Player* playerPtr_ = nullptr;
    // 由 playerPtr_ 是否?本?象?有??定析构? delete 与否
    bool ownsPlayer_ = false;

    void initFromFile(const std::string& fn, int key_count);

public:
    int keyCount;
    // 只要用?一?构造，就?了：第三???缺省可?
    // 如果 externalPlayer ? nullptr，就?部 new Player；否?复用它。
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
