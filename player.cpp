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

// ���ܦ}?����V
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

    // 1) ?���?��
    if      (d == 'W') nr--;
    else if (d == 'S') nr++;
    else if (d == 'A') nc--;
    else if (d == 'D') nc++;
    else return false;

    // 2) �V��??
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

    // 4) **��** ?�z Monster�]���]?�i? OBSTACLE type�^
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

        // ��?�� Empty�]�� delete mo itself�^
        maze->get_maze()[nr][nc] = new Empty();
        blk = maze->get_maze()[nr][nc];
        tp = blk->getType();
    }
    // 5) �I��@�� Obstacle
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

    // 6) �B?
    if (auto* sw = dynamic_cast<Sword*>(blk)) {
        sw->player_touched(*this);
        std::cout << "Got Sword\n";
        std::cout << "Press Enter to continue...";
        std::getline(std::cin, dummy);

        maze->get_maze()[nr][nc] = new Empty();
        blk = maze->get_maze()[nr][nc];
        tp = blk->getType();
    }
    // 7) �B?��
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
    // 9) �i����?
    else if (dynamic_cast<MovableGoal*>(blk)) {
        // pass
    }
    // 10) ?�e?
    else if (auto* p = dynamic_cast<Portal*>(blk)) {
        p->teleport(maze, nr, nc);
        std::cout << "Press Enter to continue...";
        std::getline(std::cin, dummy);
    }

    // 11) �M��?��m�GPortal ���`���?�A�_? new Empty
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

    // 12) ��s���a��m�}��J�ۤv
    player_row = nr;
    player_col = nc;
    maze->get_maze()[player_row][player_col] = this;

    return false;
}






// �̫O���a��?�Q���̩�J�a?
void Player::change_symbol(Maze* maze) {
    maze->get_maze()[player_row][player_col] = this;
}
