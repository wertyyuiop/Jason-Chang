// block.cpp
#include "block.h"
#include "player.h"
#include "maze.h"
#include <bits/stdc++.h>

// 构造函?：先初始化 isVisible，再初始化 type，
// ?序与成??明保持一致，消除 -Wreorder
Block::Block(BlockType t)
  : isVisible(false)
  , type(t)
{
    // 初始化 3×3 符????空格
    symbol.assign(3, vector<char>(3, ' '));
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

void Block::setVisible(bool visible) {
    isVisible = visible;
}

bool Block::getVisible() const {
    return isVisible;
}

BlockType Block::getType() const {
    return type;
}

vector<vector<char>> Block::getSymbol() const {
    return symbol;
}

// Empty 方?
Empty::Empty()
  : Block(EMPTY)
{}

// Wall 方?
Wall::Wall()
  : Block(WALL)
{
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            symbol[i][j] = '*';
}

// Goal 方?
Goal::Goal()
  : Block(GOAL)
{
    symbol[0][0] = '$'; symbol[0][2] = '$';
    symbol[1][1] = '$';
    symbol[2][0] = '$'; symbol[2][2] = '$';
}

// Key 方?
Key::Key()
  : Block(KEY)
{
    symbol[1][1] = 'K';
}

// Obstacle 方?
Obstacle::Obstacle()
  : Block(OBSTACLE)
{
    static bool seeded = false;
    if (!seeded) {
        srand((unsigned)time(nullptr));
        seeded = true;
    }
    hp = rand() % 41 + 10;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            symbol[i][j] = '@';
    symbol[1][1] = '#';
}

void Obstacle::player_touched(bool& valid, Player& player, Maze* maze) {
    // 先扣血
    hp -= player.getAtk();
    if (hp > 0) {
        valid = false;
        cout << "Obstacle takes damage. Remaining HP: " << hp << endl;
        return;
    }

    valid = true;
    cout << "Obstacle destroyed. Clearing all linked blocks..." << endl;

    int R = maze->get_nRow();
    int C = maze->get_nCol();
    vector<vector<Block*>>& grid = maze->get_maze();

    // 1) 找到 this 在 maze[][] 中的位置
    int sr = -1, sc = -1;
    for (int r = 0; r < R && sr < 0; ++r) {
        for (int c = 0; c < C; ++c) {
            if (grid[r][c] == this) {
                sr = r;
                sc = c;
                break;
            }
        }
    }
    if (sr < 0) return;  // safety

    // 2) BFS flood-fill 所有相?的 OBSTACLE
    vector<vector<bool>> seen(R, vector<bool>(C, false));
    queue<pair<int,int>> q;
    seen[sr][sc] = true;
    q.push(make_pair(sr, sc));

    const int dr[4] = {-1,1,0,0};
    const int dc[4] = {0,0,-1,1};

    while (!q.empty()) {
        pair<int,int> cur = q.front();
        q.pop();
        int r = cur.first;
        int c = cur.second;

        // 覆?? Empty（不 delete ?指?）
        grid[r][c] = new Empty();

        // ?展四?方向
        for (int d = 0; d < 4; ++d) {
            int nr = r + dr[d];
            int nc = c + dc[d];
            if (nr < 0 || nr >= R || nc < 0 || nc >= C)
                continue;
            if (seen[nr][nc])
                continue;
            if (grid[nr][nc]->getType() == OBSTACLE) {
                seen[nr][nc] = true;
                q.push(make_pair(nr, nc));
            }
        }
    }
}


