// portal.cpp
#include "portal.h"
#include "maze.h"
#include "block.h"
#include <iostream>
using namespace std;

Portal::Portal(int r, int c)
  : Block(EMPTY), row(r), col(c)
{
    symbol[1][1] = 'P';
}

void Portal::teleport(Maze* maze, int& pr, int& pc)
{
    auto& ps = maze->get_portals();
    if (ps.size() != 2) return;

    // Pick the ¡§other¡¨ portal by pointer identity
    Portal* other = (ps[0] == this ? ps[1] : ps[0]);

    int new_r = other->getRow();
    int new_c = other->getCol();

    // Bounds?check against maze dimensions
    if (new_r < 0 || new_r >= maze->get_nRow() ||
        new_c < 0 || new_c >= maze->get_nCol())
    {
        cerr << "Error: Portal target out of bounds: ("
             << new_r << "," << new_c << ")\n";
        return;
    }

    // Perform the teleport
    pr = new_r;
    pc = new_c;
    cout << "Teleported to (" << pr << "," << pc << ")\n";
}
