  // player.cpp
  #include "player.h"
  #include "block.h"
  #include <iostream>
  #include <cstdlib> // for rand()
  #include <ctime>
  using namespace std;
  
  Player::Player() : Block(PLAYER) {}
  
  char Player::get_direction() const {
      char direction;
      cout << "Enter direction (W/A/S/D, E to exit): ";
      cin >> direction;
      return toupper(direction);
  }
  
  void Player::move(char direction, Maze* maze) {
      int new_row = player_row;
      int new_col = player_col;
      switch (direction) {
          case 'W': new_row--; break;
          case 'A': new_col--; break;
          case 'S': new_row++; break;
          case 'D': new_col++; break;
          default:
              cout << "Invalid direction! Use W/A/S/D." << endl;
              return;
      }
  
      if (new_row < 0 || new_row >= maze->get_nRow() ||
          new_col < 0 || new_col >= maze->get_nCol()) {
          cout << "Out of bounds!" << endl;
          return;
      }
  
      Block* target = maze->get_maze()[new_row][new_col];
      BlockType tType = target->getType();
  
      if (tType == WALL) {
          target->player_touched();
          return;
      }
  
      if (tType == OBSTACLE) {
          Obstacle* obs = dynamic_cast<Obstacle*>(target);
          bool defeated = false;
          obs->player_touched(defeated, *this, maze);
          if (!defeated) {
              cout << "Hit obstacle! Remaining HP: " << obs->getHp() << endl;
              return;
          } else {
              cout << "Obstacle defeated!" << endl;
          }
      }
  
      if (tType == KEY) {
          cout << "Collected a key!" << endl;
          keyCollected++;
          maze->get_maze()[new_row][new_col] = new Empty();
      }
  
      if (tType == GOAL) {
          if (keyCollected > 0) {
              cout << "You reached the goal and won the game!" << endl;
              maze->printMaze();
              exit(0);
          } else {
              cout << "You need a key to finish the game!" << endl;
              return;
          }
      }
  
      maze->get_maze()[player_row][player_col] = new Empty();
      player_row = new_row;
      player_col = new_col;
      maze->get_maze()[player_row][player_col] = this;
  }
  
  void Player::change_symbol(Maze* maze) {
      maze->get_maze()[player_row][player_col] = this;
  }