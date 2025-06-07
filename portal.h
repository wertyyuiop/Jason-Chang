// portal.h
#ifndef PORTAL_H
#define PORTAL_H

#include "block.h"
class Maze;

class Portal : public Block {
private:
    int row, col;

public:
    Portal(int r, int c);
    int getRow() const { return row; }
    int getCol() const { return col; }
    void teleport(Maze* maze, int& pr, int& pc);
};

#endif // PORTAL_H
