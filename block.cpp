#include "block.h"
#include "player.h"
#include "maze.h"
#include <iostream>
using namespace std;

Block::Block(BlockType type) : type(type), isVisible(false) {
    symbol.resize(3, vector<char>(3, ' '));
    if (type == PLAYER) {
        symbol[0][1] = 'O';
        symbol[1][0] = '/'; symbol[1][1] = '|'; symbol[1][2] = '\\';
        symbol[2][0] = '/';                   symbol[2][2] = '\\';
    }
}

void Block::player_touched() {
    if (type == KEY) {
        cout << "Player picked up a key!" << endl;
        isVisible = false;
    } else if (type == WALL) {
        cout << "Player hit a wall!" << endl;
    }
}

void Block::setVisible(bool visible) { isVisible = visible; }
bool Block::getVisible() const { return isVisible; }
BlockType Block::getType() const { return type; }
vector<vector<char>> Block::getSymbol() const { return symbol; }

Empty::Empty() : Block(EMPTY) {}

Wall::Wall() : Block(WALL) {
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            symbol[i][j] = '*';
}

Goal::Goal() : Block(GOAL) {
    symbol[0][0] = '$'; symbol[0][2] = '$';
    symbol[1][1] = '$';
    symbol[2][0] = '$'; symbol[2][2] = '$';
}

Key::Key() : Block(KEY) {
    symbol[1][1] = 'K';
}

Obstacle::Obstacle() : Block(OBSTACLE) {
    static bool seeded = false;
    if (!seeded) {
        srand(time(nullptr));
        seeded = true;
    }
    hp = rand() % 41 + 10;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            symbol[i][j] = '@';
    symbol[1][1] = '#';
}

void Obstacle::player_touched(bool& valid, Player& player, Maze* maze) {
    hp -= player.getAtk();
    if (hp <= 0) {
        valid = true;
        cout << "Obstacle destroyed. Clearing all linked blocks..." << endl;
        for (int r = 0; r < maze->get_nRow(); ++r) {
            for (int c = 0; c < maze->get_nCol(); ++c) {
                Block* b = maze->get_maze()[r][c];
                if (b->getType() == OBSTACLE) {
                    Obstacle* other = dynamic_cast<Obstacle*>(b);
                    if (other && other == this) {
                        delete maze->get_maze()[r][c];
                        maze->get_maze()[r][c] = new Empty();
                    }
                }
            }
        }
    } else {
        valid = false;
        cout << "Obstacle takes damage. Remaining HP: " << hp << endl;
    }
}
