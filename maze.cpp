#include "maze.h"
#include "block.h"
#include "player.h"
#include "movable_goal.h"
#include "portal.h"
#include "sword.h"
#include "monster.h"

#include <fstream>
#include <algorithm>
#include <iomanip>
#include <vector>
#include <queue>
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

void Maze::initFromFile(const string& fn, int key_count) {
    srand((unsigned)time(nullptr));

    // 1) 用 getline ?行
    ifstream f(fn);
    vector<string> lines;
    string line;
    while (getline(f, line)) {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();
        lines.push_back(line);
    }
    nRow = (int)lines.size();

    // 2) 取最大?度
    nCol = 0;
    for (auto &l : lines)
        nCol = max(nCol, (int)l.size());

    // 3) 建 grid
    maze.assign(nRow, vector<Block*>(nCol,nullptr));
    for (int r = 0; r < nRow; ++r) {
        for (int c = 0; c < nCol; ++c) {
            char ch = (c < (int)lines[r].size() ? lines[r][c] : '0');
            switch (ch) {
                case '1': maze[r][c] = new Wall();      break;
                case '2': maze[r][c] = new Goal();      break;
                case '3': maze[r][c] = new Obstacle();  break;
                case '5': {
                    auto mg = new MovableGoal(r,c);
                    maze[r][c] = mg;
                    movableGoal  = mg;
                    break;
                }
                case '6': maze[r][c] = new Sword();     break;
                case '7': maze[r][c] = new Monster();   break;
                default:  maze[r][c] = new Empty();     break;
            }
        }
    }

    // 4) 放玩家：reuse 或 new，并重置坐?
    delete maze[1][1];
    if (playerPtr_) {
        maze[1][1] = playerPtr_;
        ownsPlayer_ = false;
    } else {
        maze[1][1] = new Player();
        ownsPlayer_ = true;
    }
    static_cast<Player*>(maze[1][1])->set_position(1,1);

    // 5) BFS 收集 EMPTY
    vector<vector<bool>> seen(nRow, vector<bool>(nCol,false));
    vector<pair<int,int>> freeSpaces;
    static const int dr[4] = {-1,1,0,0}, dc[4] = {0,0,-1,1};
    for (int sr = 0; sr < nRow; ++sr) {
        for (int sc = 0; sc < nCol; ++sc) {
            if (maze[sr][sc]->getType()!=EMPTY || seen[sr][sc]) continue;
            queue<pair<int,int>> Q;
            seen[sr][sc] = true;
            Q.push(make_pair(sr,sc));
            while (!Q.empty()) {
                auto cur = Q.front(); Q.pop();
                int r = cur.first, c = cur.second;
                freeSpaces.push_back(make_pair(r,c));
                for (int d = 0; d < 4; ++d) {
                    int nr = r + dr[d], nc = c + dc[d];
                    if (nr<0||nr>=nRow||nc<0||nc>=nCol) continue;
                    if (seen[nr][nc]) continue;
                    if (maze[nr][nc]->getType()==EMPTY) {
                        seen[nr][nc] = true;
                        Q.push(make_pair(nr,nc));
                    }
                }
            }
        }
    }

    // 6) shuffle
    random_shuffle(freeSpaces.begin(), freeSpaces.end());

    // 7) 散 2 portals
    int idx = 0;
    int numP = min(2,(int)freeSpaces.size());
    for (int i=0; i<numP; ++i, ++idx) {
        int pr = freeSpaces[idx].first;
        int pc = freeSpaces[idx].second;
        delete maze[pr][pc];
        maze[pr][pc] = new Portal(pr,pc);
        portals.push_back(static_cast<Portal*>(maze[pr][pc]));
    }

    // 8) 散 key_count keys
    int placed = 0;
    while (placed < key_count && idx < (int)freeSpaces.size()) {
        int kr = freeSpaces[idx].first;
        int kc = freeSpaces[idx].second;
        ++idx;
        if (maze[kr][kc]->getType()==EMPTY) {
            delete maze[kr][kc];
            maze[kr][kc] = new Key();
            ++placed;
        }
    }

    // 9) 分配 pixel 并首次渲染
    pixel.assign(nRow*3, vector<char>(nCol*3,' '));
    readMaze();
}

Maze::Maze(const string& fn, int key_count, Player* externalPlayer)
  : playerPtr_(externalPlayer), ownsPlayer_(false), keyCount(key_count)
{
    initFromFile(fn, key_count);
}

Maze::~Maze() {
    for (int r = 0; r < nRow; ++r) {
        for (int c = 0; c < nCol; ++c) {
            if (!ownsPlayer_ && maze[r][c] == playerPtr_) continue;
            delete maze[r][c];
        }
    }
}

void Maze::printMaze(Player* p) {
    readMaze();
    system("clear");
    int pr = p->getRow();
    int pc = p->getCol();
    if(seen.empty()) {
        for(int i=0;i<nRow;i++) {
            vector<bool> v(nCol, 0);
            seen.push_back(v);
        }
    }

    // 1) Mark the 3×3 around the player as seen
    for (int dr = -1; dr <= 1; ++dr) {
        for (int dc = -1; dc <= 1; ++dc) {
            int r = pr + dr, c = pc + dc;
            if (r >= 0 && r < nRow && c >= 0 && c < nCol) {
                seen[r][c] = true;
            }
        }
    }

    // 2) Draw only seen tiles
    system("clear");
    for (int r = 0; r < nRow; ++r) {
        for (int subRow = 0; subRow < 3; ++subRow) {
            for (int c = 0; c < nCol; ++c) {
                if (seen[r][c]) {
                    // copy the actual 3×3 pixels
                    for (int subCol = 0; subCol < 3; ++subCol) {
                        cout << pixel[r*3 + subRow][c*3 + subCol];
                    }
                } else {
                    // unseen: blank 3 columns
                    cout << "   ";
                }
            }
            cout << "\n";
        }
    }
}


void Maze::printStatus(Player* p) {
    // build the HP/ATK portion
    string status =
        "HP:"  + to_string(p->getHp())     + "/" + to_string(p->getMaxHp())
      + "  ATK:" + to_string(p->getAtk())   + "/" + to_string(p->getMaxAtk())
      + "  KEYS:" + to_string(p->getkeyCollected())
                 + "/" + to_string(keyCount);
    // align it under the map
    cout << setw(nCol*3) << status << "\n";
}

void Maze::start() {
    auto player = static_cast<Player*>(maze[1][1]);
    player->keyCollected = 0;
    while (gaming) {
        printMaze(player);
        printStatus(player);
        char d = player->get_direction();
        if (d=='E') { cout<<"Exit\n"; break; }
        gaming = !player->move(d,this);
        if (gaming == -1) {
            cout<<"You Lose!\n";
            exit(0);
        }
        player->change_symbol(this);
        if (movableGoal)
            movableGoal->move_random(this);
    }
    if (!gaming) cout<<"Game Over!\n";
}
