#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "block.h"
class Player;
class Maze;

class Obstacle : public Block {
private:
    int hp;

public:
    Obstacle();
    int getHp() const { return hp; }
    void player_touched(bool& valid, Player& player, Maze* maze);
};

#endif // OBSTACLE_H
