#include "player.h"
#include "maze.h"
#include "movable_goal.h"
#include "portal.h"
#include "sword.h"
#include "monster.h"
#include <iostream>
using namespace std;

Player::Player():Block(PLAYER){}

char Player::get_direction() const {
    char d;cout<<"Dir (WASD, E exit):";cin>>d;return toupper(d);
}

bool Player::move(char d, Maze* maze) {
    int nr = player_row, nc = player_col;
    if (d == 'W')      nr--;
    else if (d == 'S') nr++;
    else if (d == 'A') nc--;
    else if (d == 'D') nc++;
    else return false;

    if (nr < 0 || nr >= maze->get_nRow() || nc < 0 || nc >= maze->get_nCol()) {
        cout << "OOB\n";
        return false;
    }

    Block* t = maze->get_maze()[nr][nc];
    BlockType tp = t->getType();

    if (tp == WALL) {
        t->player_touched();
        return false;
    }

    if (tp == OBSTACLE) {
        auto* o = dynamic_cast<Obstacle*>(t);
        bool ok = false;
        o->player_touched(ok, *this, maze);
        if (!ok) {
            cout << "Hit! HP=" << o->getHp() << endl;
            return false;
        }
        cout << "Cleared\n";
        delete maze->get_maze()[nr][nc];
        maze->get_maze()[nr][nc] = new Empty();
        t = maze->get_maze()[nr][nc];
        tp = t->getType();
    }

    if (auto* s = dynamic_cast<Sword*>(t)) {
        s->player_touched(*this);
        delete maze->get_maze()[nr][nc];
        maze->get_maze()[nr][nc] = new Empty();
        t = maze->get_maze()[nr][nc];
        tp = t->getType();
    }
    else if (auto* m = dynamic_cast<Monster*>(t)) {
        bool ok = false;
        m->player_touched(ok, *this, maze);
        if (!ok) return false;
        delete maze->get_maze()[nr][nc];
        maze->get_maze()[nr][nc] = new Empty();
        t = maze->get_maze()[nr][nc];
        tp = t->getType();
    }
    else if (tp == KEY) {
        cout << "Key++\n";
        keyCollected++;
        delete maze->get_maze()[nr][nc];
        maze->get_maze()[nr][nc] = new Empty();
        t = maze->get_maze()[nr][nc];
        tp = t->getType();
    }
    else if (tp == GOAL) {
        // 掃描整張地圖，看看還有沒有 KEY
        bool anyKeysLeft = false;
        for (auto& row : maze->get_maze()) {
            for (auto* blk : row) {
                if (blk->getType() == KEY) {
                    anyKeysLeft = true;
                    break;
                }
            }
            if (anyKeysLeft) break;
        }
        if (!anyKeysLeft || keyCollected > 0) {
            cout << "You win!\n";
            return true;
        } else {
            cout << "Need key\n";
            return false;
        }
    }
    else if (auto* mg = dynamic_cast<MovableGoal*>(t)) {
        // 不做事，直接往下移動玩家
    }
    else if (auto* p = dynamic_cast<Portal*>(t)) {
        // 傳送並更新 nr,nc
        p->teleport(maze, nr, nc);
    }
    
    maze->get_maze()[player_row][player_col] = new Empty();

    // 更新座標並放上 this
    player_row = nr;
    player_col = nc;
    maze->get_maze()[player_row][player_col] = this;

    // Restore portals
    for (Portal* p : maze->get_portals()) {
        int pr = p->getRow(), pc = p->getCol();
        if (pr == player_row && pc == player_col) continue;
        maze->get_maze()[pr][pc] = p;
    }

    return false;
}


void Player::change_symbol(Maze* maze){
    maze->get_maze()[player_row][player_col]=this;
}
