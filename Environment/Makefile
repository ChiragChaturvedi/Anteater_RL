# Makefile for my_program

# Compiler flags
CXXFLAGS := -std=c++11 -Wall -Wextra -pedantic

# Include directories
INCLUDES := -I.

# Libraries (if any)
LIBS :=

# Source files
SRCS := main.cpp

# Object files
OBJS := $(SRCS:.cpp=.o)

# Target executable
TARGET := my_program

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) $(LIBS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) $(OBJS) $(TARGET)
