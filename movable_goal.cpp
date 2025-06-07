#include "movable_goal.h"
#include "maze.h"
#include "block.h"
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

MovableGoal::MovableGoal(int r, int c) : Goal(), row(r), col(c) {
    static bool seeded = false;
    if (!seeded) {
        srand(time(nullptr));
        seeded = true;
    }
}

void MovableGoal::move_random(Maze* maze) {
    static const vector<pair<int,int>> dirs = {{-1,0},{1,0},{0,-1},{0,1}};
    int attempts = 10;
    while (attempts--) {
        int dr = dirs[rand() % 4].first;
        int dc = dirs[rand() % 4].second;
        int nr = row + dr, nc = col + dc;
        if (nr<0||nr>=maze->get_nRow()||nc<0||nc>=maze->get_nCol()) continue;
        Block* t = maze->get_maze()[nr][nc];
        if (t->getType()==EMPTY||t->getType()==KEY) {
            delete maze->get_maze()[row][col];
            maze->get_maze()[row][col] = new Empty();
            row = nr; col = nc;
            maze->get_maze()[row][col] = this;
            break;
        }
    }
}
