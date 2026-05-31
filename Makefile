CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
SRC = src/main.cpp src/Character.cpp src/Skill.cpp src/Item.cpp src/Hero.cpp \
      src/Warrior.cpp src/Mage.cpp src/Archer.cpp src/Enemy.cpp src/Combat.cpp src/Game.cpp
OUT = rpg_game.exe

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

clean:
	del $(OUT)
