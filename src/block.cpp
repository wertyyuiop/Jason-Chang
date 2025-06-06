#include "block.h"
#include "player.h"
#include <iostream>
using namespace std;

Block::Block(BlockType type) : type(type), isVisible(false) {
    symbol.resize(3, vector<char>(3, ' '));
    if(type == PLAYER) {
        symbol[0][1] = 'O';
        symbol[1][0] = '/'; symbol[1][1] = '|';  symbol[1][2] = '\\';
        symbol[2][0] = '/'; symbol[2][2] = '\\';
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
