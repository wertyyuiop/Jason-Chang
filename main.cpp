#include <iostream>
#include <string>
#include <fstream>
#include "maze.h"
#include "player.h"

using namespace std;

int main(){
    string file = "maze2.txt";
    Maze maze(file);  
    maze.start();
    return 0;
}
