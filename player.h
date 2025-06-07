#ifndef PLAYER_H
#define PLAYER_H

#include "maze.h"
#include "portal.h"
#include "sword.h"
#include "monster.h"
#include "block.h"

class Player:public Block{
private:
    int player_row=1,player_col=1;
    int keyCollected=0; bool hasSword=false,hasKey=false;int atk=10;
public:
    Player();
    char get_direction() const;
    int getAtk() const{return atk;}
    bool getHasSword() const{return hasSword;}
    void setHasSword(bool v){hasSword=v;}
    void increaseAtk(int v){atk+=v;}
    bool move(char d,Maze* maze);
    void change_symbol(Maze* maze);
};

#endif // PLAYER_H
