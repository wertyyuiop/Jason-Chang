// maze.h
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
    std::vector<std::vector<char>>      pixel;
    int nRow = 0, nCol = 0;
    bool gaming = true;
    MovableGoal* movableGoal = nullptr;
    std::vector<Portal*> portals;

public:
    // single constructor: always computes max-width and then scatters keys
    Maze(const std::string& fn, int key_count = 0);
    ~Maze();

    int get_nRow() const { return nRow; }
    int get_nCol() const { return nCol; }
    auto& get_maze()     { return maze; }
    auto& get_portals()  { return portals; }

    void readMaze();
    void printMaze();
    void start();
};

#endif // MAZE_H
