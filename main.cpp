#include "maze.h"
#include "player.h"
#include <iostream>
#include <fstream>
using namespace std;

int main(){
    // 1) 只 new 一次玩家
    Player* hero = new Player();

    // 2) ??加? maze1.txt, maze2.txt... 直到文件不存在
    for(int i = 1; ; ++i){
        string fn = "maze" + to_string(i) + ".txt";
        ifstream fin(fn);
        if(!fin.good()) break;
        fin.close();

        // **一定要?第三??? hero！**
        Maze maze(fn, /*key_count=*/i, hero);
        maze.start();
    }

    delete hero;
    return 0;
}
