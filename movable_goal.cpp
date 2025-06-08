// movable_goal.cpp
#include "movable_goal.h"
#include "maze.h"
#include "block.h"
#include <cstdlib>
#include <ctime>
#include <vector>
using namespace std;

MovableGoal::MovableGoal(int r, int c)
  : Goal(), row(r), col(c), beneath_(new Empty())
{
    static bool seeded = false;
    if (!seeded) {
        srand((unsigned)time(nullptr));
        seeded = true;
    }
}

MovableGoal::~MovableGoal() {
    delete beneath_;
}

void MovableGoal::move_random(Maze* maze) {
    static const vector<pair<int,int>> dirs = {{-1,0},{1,0},{0,-1},{0,1}};
    int attempts = 10;
    while (attempts--) {
        int i = rand() % dirs.size();
        int nr = row + dirs[i].first;
        int nc = col + dirs[i].second;
        // 出界跳?
        if (nr<0||nr>=maze->get_nRow()||nc<0||nc>=maze->get_nCol())
            continue;

        Block* target = maze->get_maze()[nr][nc];
        BlockType tt = target->getType();

        // 只要不是?/障?/??，就允?走
        if (tt != WALL && tt != OBSTACLE && tt != GOAL && tt != PLAYER) {
            // 1) ?原?地?
            maze->get_maze()[row][col] = beneath_;
            // 2) ?住新地?，??其?型
            beneath_ = target;
            // 3) 更新位置并放自己
            row = nr; col = nc;
            maze->get_maze()[row][col] = this;
            break;
        }
    }
}

