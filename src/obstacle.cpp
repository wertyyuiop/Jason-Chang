#include "obstacle.h"
#include "player.h"
#include "maze.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

Obstacle::Obstacle() : Block(OBSTACLE) {
    static bool seeded = false;
    if (!seeded) {
        srand(time(nullptr));
        seeded = true;
    }
    hp = rand() % 41 + 10; // HP: 10~50
    symbol[1][1] = '#';
}

void Obstacle::player_touched(bool& valid, Player& player, Maze* maze) {
    hp -= player.getAtk();
    if (hp <= 0) {
        valid = true;
        cout << "Obstacle destroyed. Clearing region..." << endl;

        // 清除整塊障礙（多格支援）
        for (int i = 0; i < maze->get_nRow(); ++i) {
            for (int j = 0; j < maze->get_nCol(); ++j) {
                Block* b = maze->get_maze()[i][j];
                if (b->getType() == OBSTACLE) {
                    Obstacle* other = dynamic_cast<Obstacle*>(b);
                    if (other && other == this) {
                        maze->get_maze()[i][j] = new Empty();
                    }
                }
            }
        }
    } else {
        valid = false;
        cout << "Obstacle takes damage. Remaining HP: " << hp << endl;
    }
}
