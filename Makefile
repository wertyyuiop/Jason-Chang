.PHONY: all clean run

# Compiler settings
CXX = g++
CXXFLAGS = -I ./inc -std=c++11
WARNINGS = -g -Wall

# Source files and object files
SRCDIR = src
OBJDIR = obj
SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))

# Name of the executable
TARGET = brave_tour

all: $(TARGET)

$(OBJDIR):
	@mkdir -p $(OBJDIR)  # Create directory if it doesn't exist

$(TARGET): main.cpp $(OBJS)
	$(CXX) $(WARNINGS) $(CXXFLAGS) $^ -o $@

# Compilation rule for object files with automatic dependency generation
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR) Makefile
	$(CXX) $(WARNINGS) $(CXXFLAGS) -c $< -o $@

# Run the program
run:
	./$(TARGET) $(cell) $(opt) $(output)

# Clean rule
clean:
	@echo "Cleaning up..."
	rm -rf $(OBJDIR)/*.o $(TARGET)

cfile: