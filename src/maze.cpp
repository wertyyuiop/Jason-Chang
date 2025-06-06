// maze.cpp
#include "maze.h"
#include "block.h"
#include "player.h"
#include <fstream>
#include <iostream>
#include <termios.h>
#include <unistd.h>

void Maze::readMaze() {
    vector<vector<char>> block;
    for(int bRow = 0; bRow < nRow; bRow++) {
        for(int bCol = 0; bCol < nCol; bCol++){  
            block = maze[bRow][bCol]->getSymbol();
            for(int i=0; i<3; i++) {
                for(int j=0; j<3; j++) {
                    pixel[bRow*3 + i][bCol*3 + j] = block[i][j];
                }
            }   
        }
    }
}

Maze::Maze(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        return;
    }

    string s;
    while(getline(file, s)) {
        nCol = s.length();
        maze.push_back(vector<Block*>());

        for (int j = 0; j < nCol; ++j) {
            char ch = s[j];
            if (ch == '0') maze[nRow].push_back(new Empty());
            else if (ch == '1') maze[nRow].push_back(new Wall());
            else if (ch == '2') maze[nRow].push_back(new Goal());
            else if (ch == '3') maze[nRow].push_back(new Obstacle());
            else if (ch == '4') maze[nRow].push_back(new Key());
            else maze[nRow].push_back(new Empty());
        }
        nRow++;
    }

    delete maze[1][1];
    maze[1][1] = new Player();
    pixel.resize(nRow * 3, vector<char>(nCol * 3, ' '));
    readMaze();
    file.close();
}

Maze::~Maze() {
    for (int i = 0; i < nRow; ++i)
        for (int j = 0; j < nCol; ++j)
            delete maze[i][j];
}

void Maze::printMaze() {
    readMaze();
    system("clear");
    for (int i = 0; i < nRow * 3; i++) {
        for (int j = 0; j < nCol * 3; j++) {
            cout << pixel[i][j];
        }
        cout << endl;
    }
}

void Maze::start() {
    Player* player = static_cast<Player*>(maze[1][1]);
    while (gaming) {
        printMaze();
        char direction = player->get_direction();
        if (direction == 'E') {
            cout << "Exiting..." << endl;
            break;
        }
        player->move(direction, this);
        player->change_symbol(this);
    }
}
