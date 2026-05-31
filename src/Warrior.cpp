#include "../include/Warrior.h"
#include "../include/Skill.h"
#include <iostream>

Warrior::Warrior(const std::string& name)
    : Hero(name, 150, 30, 20, 15, 8) {
    skills.push_back(std::make_shared<PhysicalSkill>(
        "Frappe Puissante", 1.5f, "Attaque physique x1.5"));
    skills.push_back(std::make_shared<PhysicalSkill>(
        "Coup de Bouclier", 0.8f, "Attaque + réduit les dégâts"));
    skills.push_back(std::make_shared<MultiHitSkill>(
        "Tourbillon", 3, 0.6f, "3 coups rapides"));
    skills.push_back(std::make_shared<HealSkill>(
        "Second Souffle", 40, 20, "Récupère 40 PV"));
}

void Warrior::levelUp() {
    Hero::levelUp();
    maxHp    += 20;
    hp        = maxHp;
    attack   += 4;
    defense  += 3;
    maxMana  += 5;
    mana      = maxMana;
    std::cout << "  +20 PV max, +4 ATK, +3 DEF\n";
}
