#include "maze.h"
#include "player.h"
#include <iostream>
#include <fstream>
using namespace std;

int main(){
    int i = 0;
    while(++i) {
        string s = "maze"+to_string(i)+".txt";
        ifstream fin;
        fin.open(s);
        if(!fin.good()) break;
        Maze maze("maze"+to_string(i)+".txt", i);
        maze.start();
    }
    return 0;
}
