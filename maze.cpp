// maze.cpp
#include "maze.h"
#include "block.h"
#include "player.h"
#include "movable_goal.h"
#include "portal.h"
#include "sword.h"
#include "monster.h"
#include <bits/stdc++.h>
using namespace std;

void Maze::readMaze() {
    for (int r = 0; r < nRow; ++r) {
        for (int c = 0; c < nCol; ++c) {
            auto sym = maze[r][c]->getSymbol();
            for (int i = 0; i < 3; ++i)
                for (int j = 0; j < 3; ++j)
                    pixel[r*3 + i][c*3 + j] = sym[i][j];
        }
    }
}

Maze::Maze(const string& fn, int key_count) {
    srand((unsigned)time(nullptr));

    // 1) 先?入所有行
    ifstream f(fn);
    vector<string> lines;
    string line;
    while (f >> line) lines.push_back(line);
    nRow = (int)lines.size();

    // 2) 取最大?度
    nCol = 0;
    for (auto &l : lines)
        if ((int)l.size() > nCol)
            nCol = (int)l.size();

    // 3) 填充 maze 网格（超出原 line ?度的列? Empty）
    maze.assign(nRow, vector<Block*>(nCol, nullptr));
    for (int r = 0; r < nRow; ++r) {
        for (int c = 0; c < nCol; ++c) {
            char ch = (c < (int)lines[r].size() ? lines[r][c] : '0');
            switch (ch) {
                case '1': maze[r][c] = new Wall();        break;
                case '2': maze[r][c] = new Goal();        break;
                case '3': maze[r][c] = new Obstacle();    break;
                case '5': { auto mg = new MovableGoal(r,c);
                           maze[r][c] = mg;
                           movableGoal = mg;
                           break;
                         }
                case '6': maze[r][c] = new Sword();       break;
                case '7': maze[r][c] = new Monster();     break;
                default:  maze[r][c] = new Empty();       break;
            }
        }
    }

    // 4) 放 Player 在 (1,1)
    delete maze[1][1];
    maze[1][1] = new Player();

    // 1) 用 BFS 遍?所有?通?，收集所有 EMPTY 格子
    vector<vector<bool>> seen(nRow, vector<bool>(nCol, false));
    vector<pair<int,int>> freeSpaces;
    
    static const int dr[4] = {-1,1,0,0};
    static const int dc[4] = {0,0,-1,1};
    
    for (int sr = 0; sr < nRow; ++sr) {
        for (int sc = 0; sc < nCol; ++sc) {
            if (maze[sr][sc]->getType() != EMPTY) continue;
            if (seen[sr][sc]) continue;
    
            // 新?通?，? (sr,sc) BFS
            queue<pair<int,int>> Q;
            seen[sr][sc] = true;
            Q.push(make_pair(sr,sc));
    
            while (!Q.empty()) {
                auto cur = Q.front(); Q.pop();
                int r = cur.first, c = cur.second;
                // ????空格
                freeSpaces.push_back(make_pair(r,c));
    
                // 四方向?展
                for (int d = 0; d < 4; ++d) {
                    int nr = r + dr[d], nc = c + dc[d];
                    if (nr < 0 || nr >= nRow || nc < 0 || nc >= nCol) 
                        continue;
                    if (seen[nr][nc]) 
                        continue;
                    if (maze[nr][nc]->getType() == EMPTY) {
                        seen[nr][nc] = true;
                        Q.push(make_pair(nr,nc));
                    }
                }
            }
        }
    }
    
    // 2) ?机打?所有 EMPTY
    std::random_shuffle(freeSpaces.begin(), freeSpaces.end());
    
    // 3) 取前 2 ?放 Portal
    int idx = 0;
    int numPortals = min(2, (int)freeSpaces.size());
    for (int i = 0; i < numPortals; ++i, ++idx) {
        int pr = freeSpaces[idx].first;
        int pc = freeSpaces[idx].second;
        delete maze[pr][pc];
        maze[pr][pc] = new Portal(pr, pc);
        portals.push_back(static_cast<Portal*>(maze[pr][pc]));
    }
    
    // 4) 再取接下?恰好 key_count ?放 Key
    int placedKeys = 0;
    while (placedKeys < key_count && idx < (int)freeSpaces.size()) {
        int kr = freeSpaces[idx].first;
        int kc = freeSpaces[idx].second;
        ++idx;
        // 如果那里已?放了 Portal，就跳?
        if (maze[kr][kc]->getType() == EMPTY) {
            delete maze[kr][kc];
            maze[kr][kc] = new Key();
            ++placedKeys;
        }
    }
    
    // 5) 最后按最大?度分配 pixel ??并首次渲染
    pixel.assign(nRow*3, vector<char>(nCol*3, ' '));
    readMaze();
}


Maze::~Maze() {
    for (int r = 0; r < nRow; ++r)
        for (int c = 0; c < nCol; ++c)
            delete maze[r][c];
}

void Maze::printMaze() {
    readMaze();
    system("clear");
    for (int i = 0; i < nRow*3; ++i) {
        for (int j = 0; j < nCol*3; ++j)
            cout << pixel[i][j];
        cout << "\n";
    }
}

void Maze::start(){
    auto player = static_cast<Player*>(maze[1][1]);
    while (gaming) {
        printMaze();
        char d = player->get_direction();
        if (d == 'E') { cout << "Exit\n"; break; }
        gaming = !player->move(d,this);
        player->change_symbol(this);
        if (movableGoal) movableGoal->move_random(this);
    }
    if (!gaming) cout << "Game Over!\n";
}
