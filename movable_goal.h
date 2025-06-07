#ifndef MOVABLE_GOAL_H
#define MOVABLE_GOAL_H

#include "block.h"
class Maze;

class MovableGoal : public Goal {
private:
    int row, col;

public:
    MovableGoal(int r, int c);
    void move_random(Maze* maze);
    int getRow() const { return row; }
    int getCol() const { return col; }
};

#endif // MOVABLE_GOAL_H
