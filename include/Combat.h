#pragma once
#include "Hero.h"
#include "Enemy.h"

class Combat {
    Hero&  hero;
    Enemy& enemy;
    int    floor;

    void heroTurn();
    void enemyTurn();
    void displayCombatStatus() const;
    void displaySkillMenu() const;
    void displayItemMenu() const;

public:
    Combat(Hero& hero, Enemy& enemy, int floor = 1);
    bool run();
};
