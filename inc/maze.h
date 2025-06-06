#ifndef MAZE_H
#define MAZE_H

#include "block.h"
#include <vector>
#include <string>
class Player;

using namespace std;

class Maze {
private:
    vector<vector<Block*>> maze;
    vector<vector<char>> pixel;
    int nRow = 0, nCol = 0;
    int nKey = 1;
    bool gaming = true;

public:
    Maze(const string& filename);
    ~Maze();
    int get_nRow() const { return nRow; }
    int get_nCol() const { return nCol; }
    vector<vector<Block*>>& get_maze() { return maze; }

    void readMaze();
    void printMaze();
    void start();
};

#endif // MAZE_H
