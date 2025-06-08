// player.h
#ifndef PLAYER_H
#define PLAYER_H

#include "maze.h"
#include "portal.h"
#include "sword.h"
#include "monster.h"
#include "block.h"

class Player : public Block {
private:
    int player_row   = 1;
    int player_col   = 1;
    bool hasSword    = false;
    bool hasKey      = false;
    int atk          = 10;
    int hp           = 100;

public:
    int keyCollected = 0;
    Player();

    // ?�����a?�J��V�]WASD/E�^
    char get_direction() const;

    // ?�沾???�A�J??��^ true
    int move(char d, Maze* maze);

    // ? this ��^�g?�]�קK��??���^
    void change_symbol(Maze* maze);

    // �X�X ��??���m���a��? �X�X  
    void set_position(int r, int c) {
        player_row = r;
        player_col = c;
    }

    // ?�ʦs����
    int getRow()     const { return player_row;        }
    int getCol()     const { return player_col;        }
    int getHp()     const { return hp;        }
    int getMaxHp()  const { return 100;       }
    int getAtk()    const { return atk;       }
    int getMaxAtk() const { return 50;        }
    int getkeyCollected() const { return keyCollected;        }
    bool getHasSword() const { return hasSword; }
    void setHasSword(bool v) { hasSword = v;    }
    void increaseAtk(int v)  { atk += v;        }
};

#endif // PLAYER_H
