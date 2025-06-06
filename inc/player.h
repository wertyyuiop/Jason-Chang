#ifndef PLAYER_H
#define PLAYER_H

#include "maze.h"
class Block;

class Player : public Block {
private:
    int player_row = 1;
    int player_col = 1;
    int keyCollected = 0;
    int atk = 10;

public:
    Player();
    char get_direction() const;
    int getRow() const { return player_row; }
    int getCol() const { return player_col; }
    int getAtk() const { return atk; }

    void move(char direction, Maze* maze);
    void change_symbol(Maze* maze);
};

#endif // PLAYER_H
