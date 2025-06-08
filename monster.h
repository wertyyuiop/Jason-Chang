#ifndef MONSTER_H
#define MONSTER_H

#include "block.h"
class Player; class Maze;

class Monster:public Block{
public:
    Monster();
    void player_touched(bool& canPass,Player& player,Maze* maze);
    int getHp() const { return hp; }
private:
    int hp;
};

#endif // MONSTER_H
