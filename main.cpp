#include <iostream>
#include <string>
#include "maze.h"
#include "player.h"

int main(){
    string file = "maze2.txt";
    Maze maze(file);  
    maze.start();
    return 0;
}
