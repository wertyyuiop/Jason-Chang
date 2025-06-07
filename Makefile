CXX = g++
CXXFLAGS = -std=c++14 -Wall

OBJS = main.o maze.o player.o block.o sword.o monster.o movable_goal.o portal.o

all: maze_game

maze_game: $(OBJS)
	$(CXX) $(CXXFLAGS) -o maze_game $(OBJS)

main.o: main.cpp maze.h player.h
	$(CXX) $(CXXFLAGS) -c main.cpp

maze.o: maze.cpp maze.h block.h player.h sword.h monster.h movable_goal.h portal.h
	$(CXX) $(CXXFLAGS) -c maze.cpp

player.o: player.cpp player.h maze.h sword.h monster.h portal.h movable_goal.h	
	$(CXX) $(CXXFLAGS) -c player.cpp

block.o: block.cpp block.h
	$(CXX) $(CXXFLAGS) -c block.cpp

sword.o: sword.cpp sword.h player.h
	$(CXX) $(CXXFLAGS) -c sword.cpp

monster.o: monster.cpp monster.h player.h maze.h
	$(CXX) $(CXXFLAGS) -c monster.cpp

movable_goal.o: movable_goal.cpp movable_goal.h maze.h block.h
	$(CXX) $(CXXFLAGS) -c movable_goal.cpp

portal.o: portal.cpp portal.h maze.h
	$(CXX) $(CXXFLAGS) -c portal.cpp

clean:
	rm -f *.o maze_game
