  #include "block.h"
  #include "player.h"
  #include "maze.h"
  #include <iostream>
  #include <cstdlib>
  using namespace std;
  
  Obstacle::Obstacle() : Block(OBSTACLE) {
      static bool seeded = false;
      if (!seeded) {
          srand(time(nullptr));
          seeded = true;
      }
      hp = rand() % 41 + 10; // 10~50 隨機
      symbol[1][1] = '#';
  }
  
  void Obstacle::player_touched(bool& valid, Player& player, Maze* maze) {
      hp -= player.getAtk();
      if (hp <= 0) {
          valid = true;
          cout << "Obstacle destroyed. Clearing region..." << endl;
  
          // 移除整塊 obstacle（假設為 2x2）範圍內同 ID
          for (int i = 0; i < maze->get_nRow(); ++i) {
              for (int j = 0; j < maze->get_nCol(); ++j) {
                  Block* b = maze->get_maze()[i][j];
                  if (b->getType() == OBSTACLE) {
                      Obstacle* other = dynamic_cast<Obstacle*>(b);
                      if (other && other == this) {
                          maze->get_maze()[i][j] = new Empty();
                      }
                  }
              }
          }
      } else {
          valid = false;
          cout << "Obstacle takes damage. Remaining HP: " << hp << endl;
      }
  }
