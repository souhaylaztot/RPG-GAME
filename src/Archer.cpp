#include "../include/Archer.h"
#include "../include/Skill.h"
#include <iostream>

Archer::Archer(const std::string& name)
    : Hero(name, 100, 60, 15, 8, 15) {
    skills.push_back(std::make_shared<PhysicalSkill>(
        "Tir Précis", 1.3f, "Tir ciblé x1.3"));
    skills.push_back(std::make_shared<MultiHitSkill>(
        "Pluie de Flèches", 4, 0.5f, "4 flèches rapides"));
    skills.push_back(std::make_shared<MagicSkill>(
        "Flèche de Terre", 35, Element::EARTH, 15, "Flèche élémentaire"));
    skills.push_back(std::make_shared<HealSkill>(
        "Herbes Médicinales", 30, 10, "Récupère 30 PV"));
}

void Archer::levelUp() {
    Hero::levelUp();
    maxHp   += 12;
    hp       = maxHp;
    attack  += 3;
    speed   += 2;
    maxMana += 10;
    mana     = maxMana;
    std::cout << "  +12 PV max, +3 ATK, +2 VIT\n";
}
