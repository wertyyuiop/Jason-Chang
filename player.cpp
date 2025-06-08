// player.cpp
#include "player.h"
#include "maze.h"
#include "movable_goal.h"
#include "portal.h"
#include "sword.h"
#include "monster.h"
#include <iostream>
using namespace std;

Player::Player()
  : Block(PLAYER),
    player_row(1), player_col(1),
    hasSword(false), hasKey(false),
    atk(10), hp(100),
    keyCollected(0)
{}

// 提示并?取方向
char Player::get_direction() const {
    char d;
    cout << "Dir (WASD, E exit): ";
    cin >> d;
    string s;
    getline(cin, s);  // consume the rest of the line (including the newline)
    return toupper(d);
}


int Player::move(char d, Maze* maze) {
    std::string dummy;
    int nr = player_row, nc = player_col;

    // 1) ?算目?格
    if      (d == 'W') nr--;
    else if (d == 'S') nr++;
    else if (d == 'A') nc--;
    else if (d == 'D') nc++;
    else return false;

    // 2) 越界??
    if (nr < 0 || nr >= maze->get_nRow() ||
        nc < 0 || nc >= maze->get_nCol()) {
        std::cout << "OOB\n";
        std::cout << "Press Enter to continue...";
        std::getline(std::cin, dummy);
        return false;
    }

    Block* blk = maze->get_maze()[nr][nc];
    BlockType tp = blk->getType();

    // 3) ?
    if (tp == WALL) {
        blk->player_touched();
        std::cout << "Press Enter to continue...";
        std::getline(std::cin, dummy);
        return false;
    }

    // 4) **先** ?理 Monster（它也?告? OBSTACLE type）
    if (auto* mo = dynamic_cast<Monster*>(blk)) {
        if (!getHasSword()) {
          cout << "Monster blocks your way!\n";
          return -1;
        }
        bool ok = false;
        mo->player_touched(ok, *this, maze);
        if (!ok) {
            // always safe to call getHp() on mo
            std::cout << "Hit! HP=" << mo->getHp() << "\n";
            std::cout << "Press Enter to continue...";
            std::getline(std::cin, dummy);
            return false;
        }
        std::cout << "Monster defeated\n";
        std::cout << "Press Enter to continue...";
        std::getline(std::cin, dummy);

        // 覆?成 Empty（不 delete mo itself）
        maze->get_maze()[nr][nc] = new Empty();
        blk = maze->get_maze()[nr][nc];
        tp = blk->getType();
    }
    // 5) 碰到一般 Obstacle
    else if (tp == OBSTACLE) {
        auto* ob = dynamic_cast<Obstacle*>(blk);
        bool ok = false;
        ob->player_touched(ok, *this, maze);
        if (!ok) {
            std::cout << "Hit! HP=" << ob->getHp() << "\n";
            std::cout << "Press Enter to continue...";
            std::getline(std::cin, dummy);
            return false;
        }
        std::cout << "Cleared\n";
        std::cout << "Press Enter to continue...";
        std::getline(std::cin, dummy);

        maze->get_maze()[nr][nc] = new Empty();
        blk = maze->get_maze()[nr][nc];
        tp = blk->getType();
    }

    // 6) 拾?
    if (auto* sw = dynamic_cast<Sword*>(blk)) {
        sw->player_touched(*this);
        std::cout << "Got Sword\n";
        std::cout << "Press Enter to continue...";
        std::getline(std::cin, dummy);

        maze->get_maze()[nr][nc] = new Empty();
        blk = maze->get_maze()[nr][nc];
        tp = blk->getType();
    }
    // 7) 拾?匙
    else if (tp == KEY) {
        std::cout << "Key++\n";
        std::cout << "Press Enter to continue...";
        std::getline(std::cin, dummy);
        keyCollected++;

        maze->get_maze()[nr][nc] = new Empty();
        blk = maze->get_maze()[nr][nc];
        tp = blk->getType();
    }
    // 8) ??
    else if (tp == GOAL) {
        if (maze->keyCount == keyCollected)
          std::cout << "You win!\n";
        else
          std::cout << "Need key\n";

        std::cout << "Press Enter to continue...";
        std::getline(std::cin, dummy);
        return maze->keyCount == keyCollected;
    }
    // 9) 可移目?
    else if (dynamic_cast<MovableGoal*>(blk)) {
        // pass
    }
    // 10) ?送?
    else if (auto* p = dynamic_cast<Portal*>(blk)) {
        p->teleport(maze, nr, nc);
        std::cout << "Press Enter to continue...";
        std::getline(std::cin, dummy);
    }

    // 11) 清空?位置：Portal 恢复其指?，否? new Empty
    {
        bool restored = false;
        for (Portal* port : maze->get_portals()) {
            if (port->getRow() == player_row &&
                port->getCol() == player_col)
            {
                maze->get_maze()[player_row][player_col] = port;
                restored = true;
                break;
            }
        }
        if (!restored) {
            maze->get_maze()[player_row][player_col] = new Empty();
        }
    }

    // 12) 更新玩家位置并放入自己
    player_row = nr;
    player_col = nc;
    maze->get_maze()[player_row][player_col] = this;

    return false;
}






// 确保玩家符?被正确放入地?
void Player::change_symbol(Maze* maze) {
    maze->get_maze()[player_row][player_col] = this;
}
