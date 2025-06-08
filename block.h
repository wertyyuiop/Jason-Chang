// block.h
#ifndef BLOCK_H
#define BLOCK_H

#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Player;
class Maze;

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
    // ? type 放在 isVisible 之后，和下面初始化列表?序一致
    bool      isVisible;
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

class Obstacle : public Block {
private:
    int hp;

public:
    Obstacle();
    int getHp() const { return hp; }
    void player_touched(bool& valid, Player& player, Maze* maze);
};

#endif // BLOCK_H
