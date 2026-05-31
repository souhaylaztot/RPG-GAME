#include "../include/Mage.h"
#include "../include/Skill.h"
#include <iostream>

Mage::Mage(const std::string& name)
    : Hero(name, 80, 120, 10, 5, 10) {
    skills.push_back(std::make_shared<MagicSkill>(
        "Boule de Feu", 45, Element::FIRE, 20, "Attaque de feu"));
    skills.push_back(std::make_shared<MagicSkill>(
        "Tempête de Glace", 40, Element::ICE, 18, "Attaque de glace"));
    skills.push_back(std::make_shared<MagicSkill>(
        "Foudre", 55, Element::LIGHTNING, 25, "Attaque électrique"));
    skills.push_back(std::make_shared<HealSkill>(
        "Soin Magique", 50, 15, "Récupère 50 PV"));
}

void Mage::levelUp() {
    Hero::levelUp();
    maxHp   += 8;
    hp       = maxHp;
    attack  += 2;
    maxMana += 20;
    mana     = maxMana;
    std::cout << "  +8 PV max, +2 ATK, +20 Mana max\n";
}
