# Compiler settings
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

# Directories
SRC_DIR = src
OBJ_DIR = build

# Files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))
RC = src/resource.rc
RES = $(OBJ_DIR)/resource.o
TARGET = CodesysWebotsBridge.exe

# Targets
all: $(TARGET)

$(TARGET): $(OBJS) $(RES)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(RES)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(RES): $(RC)
	windres $< -O coff -o $@

clean:
	del /Q build\*.o $(TARGET)

.PHONY: all clean
