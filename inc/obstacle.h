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

    // 玩家攻擊後會回傳是否打倒該障礙
    void player_touched(bool& valid, Player& player, Maze* maze);
};

#endif // OBSTACLE_H
