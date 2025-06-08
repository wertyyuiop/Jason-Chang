// movable_goal.h
#ifndef MOVABLE_GOAL_H
#define MOVABLE_GOAL_H

#include "block.h"
class Maze;
class Block;

class MovableGoal : public Goal {
private:
    int row, col;
    Block* beneath_;    // what was under us

public:
    MovableGoal(int r, int c);
    ~MovableGoal();

    void move_random(Maze* maze);
    int getRow() const { return row; }
    int getCol() const { return col; }
};

#endif // MOVABLE_GOAL_H
