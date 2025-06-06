CXX = g++
CXXFLAGS = -std=c++17 -Wall
OBJS = main.o maze.o player.o block.o obstacle.o

all: maze_game

maze_game: $(OBJS)
	$(CXX) $(CXXFLAGS) -o maze_game $(OBJS)

main.o: main.cpp maze.h player.h
	$(CXX) $(CXXFLAGS) -c main.cpp

maze.o: maze.cpp maze.h block.h player.h obstacle.h
	$(CXX) $(CXXFLAGS) -c maze.cpp

player.o: player.cpp player.h maze.h obstacle.h
	$(CXX) $(CXXFLAGS) -c player.cpp

block.o: block.cpp block.h
	$(CXX) $(CXXFLAGS) -c block.cpp

obstacle.o: obstacle.cpp obstacle.h player.h maze.h
	$(CXX) $(CXXFLAGS) -c obstacle.cpp

clean:
	rm -f *.o maze_game
