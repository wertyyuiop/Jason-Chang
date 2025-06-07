#include "sword.h"
#include "player.h"
#include <iostream>
using namespace std;

Sword::Sword():Block(KEY){symbol[1][1]='!';}

void Sword::player_touched(Player& player){
    player.increaseAtk(10);
    player.setHasSword(true);
    cout<<"Picked up sword! ATK="<<player.getAtk()<<endl;
    isVisible=false;
}
