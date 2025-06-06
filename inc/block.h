#ifndef BLOCK_H
#define BLOCK_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Player;

enum BlockType {
    EMPTY,
    WALL,
    KEY,
    OBSTACLE,
    PLAYER,
    GOAL
};

class Block {
protected:
    vector<vector<char>> symbol;
    bool isVisible;
    BlockType type;

public:
    Block(BlockType type = EMPTY);
    virtual void player_touched();
    void setVisible(bool visible);
    bool getVisible() const;
    BlockType getType() const;
    vector<vector<char>> getSymbol() const;
    virtual ~Block() = default;
};

class Empty : public Block {
public:
    Empty();
};

class Wall : public Block {
public:
    Wall();
};

class Goal : public Block {
public:
    Goal();
};

class Key : public Block {
public:
    Key();
};

#endif // BLOCK_H
