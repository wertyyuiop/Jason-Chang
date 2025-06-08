#include "maze.h"
#include "player.h"
#include <iostream>
#include <fstream>
using namespace std;

int main(){
    // 1) �u new �@�����a
    Player* hero = new Player();

    // 2) ??�[? maze1.txt, maze2.txt... �����󤣦s�b
    for(int i = 1; ; ++i){
        string fn = "maze" + to_string(i) + ".txt";
        ifstream fin(fn);
        if(!fin.good()) break;
        fin.close();

        // **�@�w�n?�ĤT??? hero�I**
        Maze maze(fn, /*key_count=*/i, hero);
        maze.start();
    }

    delete hero;
    return 0;
}
