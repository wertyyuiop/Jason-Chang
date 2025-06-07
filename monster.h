#ifndef MONSTER_H
#define MONSTER_H

#include "block.h"
class Player; class Maze;

class Monster:public Block{
public:
    Monster();
    void player_touched(bool& canPass,Player& player,Maze* maze);
};

#endif // MONSTER_H
