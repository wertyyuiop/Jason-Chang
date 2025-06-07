#ifndef SWORD_H
#define SWORD_H

#include "block.h"
class Player;

class Sword:public Block{
public:
    Sword();
    void player_touched(Player& player);
};

#endif // SWORD_H
