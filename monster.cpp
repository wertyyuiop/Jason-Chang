// monster.cpp
#include "monster.h"
#include "player.h"
#include "maze.h"
#include <iostream>
using namespace std;

Monster::Monster()
  : Block(OBSTACLE), hp(30)  // give monsters some starting HP
{
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            symbol[i][j] = 'M';
}

void Monster::player_touched(bool& canPass, Player& player, Maze* /*maze*/) {
    // 1) If the player has no sword, block the way

    // 2) Player hits with sword
    hp -= player.getAtk();
    if (hp > 0) {
        // still alive
        canPass = false;
        cout << "Monster takes damage. Remaining HP: " << hp << "\n";
        return;
    }

    // 3) Monster dies
    canPass = true;
    cout << "Monster slain!\n";
}

// No need for isAlive() or isVisible; Player::move will delete the cell when canPass == true.
