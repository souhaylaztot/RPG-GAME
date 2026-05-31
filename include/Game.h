#pragma once
#include "Hero.h"
#include "Armor.h"
#include <memory>

class Game {
    std::shared_ptr<Hero> hero;
    int  floor;
    int  killCount;

    std::shared_ptr<Hero> createHero();
    void showMainMenu();
    bool playFloor();
    void giveArmorReward();
    void giveBossLoot(bool isOptional);
    void checkSetBonus();

public:
    Game();
    void run();
};
