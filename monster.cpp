#include "monster.h"
#include "player.h"
#include "maze.h"
#include <iostream>
using namespace std;

Monster::Monster():Block(OBSTACLE){
    for(int i=0;i<3;i++)for(int j=0;j<3;j++)symbol[i][j]='M';
}

void Monster::player_touched(bool& canPass,Player& player,Maze* maze){
    if(player.getHasSword()){
        canPass=true;
        cout<<"Monster slain!"<<endl;
        isVisible=false;
    } else {
        canPass=false;
        cout<<"Monster blocks your way!"<<endl;
    }
}
