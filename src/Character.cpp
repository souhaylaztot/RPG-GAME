#include "../include/Character.h"
#include "../include/Skill.h"
#include "../include/Color.h"
#include <iostream>
#include <algorithm>

Character::Character(const std::string& name, int hp, int atk, int def, int spd)
    : name(name), hp(hp), maxHp(hp), attack(atk), defense(def), speed(spd), level(1) {}

void Character::takeDamage(int dmg) {
    int actual = std::max(0, dmg - defense / 2);
    hp = std::max(0, hp - actual);
    std::cout << Color::boldColor(name, getClassColor())
              << Color::RESET << " reçoit "
              << Color::boldColor(std::to_string(actual), Color::BRIGHT_RED)
              << " dégâts ! "
              << makeBar(hp, maxHp, 15, Color::BRIGHT_GREEN)
              << " " << hp << "/" << maxHp << "\n";
}

void Character::heal(int amount) {
    hp = std::min(maxHp, hp + amount);
    std::cout << Color::boldColor(name, getClassColor())
              << Color::RESET << " récupère "
              << Color::boldColor(std::to_string(amount), Color::BRIGHT_GREEN)
              << " PV ! "
              << makeBar(hp, maxHp, 15, Color::BRIGHT_GREEN)
              << " " << hp << "/" << maxHp << "\n";
}

bool Character::isAlive() const { return hp > 0; }

void Character::displayStats() const {
    std::string c = getClassColor();
    std::cout << Color::BOLD << c
              << "[ " << name << " | Niv." << level << " ]" << Color::RESET
              << "  PV:" << makeBar(hp, maxHp, 12, Color::BRIGHT_GREEN)
              << Color::BRIGHT_WHITE << hp << "/" << maxHp << Color::RESET
              << "  ATK:" << Color::boldColor(std::to_string(attack),  Color::BRIGHT_RED)
              << "  DEF:" << Color::boldColor(std::to_string(defense), Color::BRIGHT_BLUE)
              << "  VIT:" << Color::boldColor(std::to_string(speed),   Color::BRIGHT_YELLOW)
              << "\n";
}
