#include "../include/Item.h"
#include "../include/Hero.h"
#include "../include/Color.h"
#include <iostream>
#include <algorithm>

void HealthPotion::use(Character& target) {
    target.heal(healAmount);
    std::cout << Color::colorize("  Vous buvez une Potion de Vie !", Color::BRIGHT_GREEN) << "\n";
}

void ManaPotion::use(Character& target) {
    Hero* h = dynamic_cast<Hero*>(&target);
    if (h) {
        int r = std::min(manaAmount, h->getMaxMana() - h->getMana());
        h->setMana(h->getMana() + r);
        std::cout << Color::colorize("  Vous buvez une Potion de Mana ! (+" + std::to_string(r) + " PM)", Color::BRIGHT_BLUE) << "\n";
    }
}

void ElixirPotion::use(Character& target) {
    target.heal(100);
    Hero* h = dynamic_cast<Hero*>(&target);
    if (h) {
        int r = std::min(50, h->getMaxMana() - h->getMana());
        h->setMana(h->getMana() + r);
        std::cout << Color::colorize("  Vous buvez un Elixir ! (+100 PV, +50 PM)", Color::BRIGHT_MAGENTA) << "\n";
    }
}

void PhoenixFeather::use(Character& target) {
    // Grants a one-time resurrection — stored as a flag via max HP boost
    target.heal(target.getMaxHp() / 2);
    std::cout << Color::boldColor("  La Plume de Phenix brule ! Vous etes ressuscite !", Color::BRIGHT_RED) << "\n";
}

void DragonBlood::use(Character& target) {
    target.setAttack(target.getAttack() + 30);
    std::cout << Color::boldColor("  Sang de Dragon ! +30 ATK !", Color::BRIGHT_RED) << "\n";
}

void SoulCrystal::use(Character& target) {
    target.heal(200);
    Hero* h = dynamic_cast<Hero*>(&target);
    if (h) {
        int r = std::min(100, h->getMaxMana() - h->getMana());
        h->setMana(h->getMana() + r);
    }
    std::cout << Color::boldColor("  Le Cristal d'Ame pulse ! +200 PV, +100 PM !", Color::BRIGHT_CYAN) << "\n";
}

AncientScroll::AncientScroll(int elem, int qty)
    : Item("Parchemin Ancien", "Boost elementaire", qty), element(elem) {
    static const std::string names[] = {"", "Feu", "Glace", "Foudre", "Terre"};
    static const std::string descs[] = {"", "+25 ATK Feu", "+20 DEF Glace", "+15 VIT Foudre", "+30 PV Terre"};
    if (elem >= 1 && elem <= 4) {
        name        = "Parchemin de " + names[elem];
        description = descs[elem];
    }
}

void AncientScroll::use(Character& target) {
    switch (element) {
        case 1: target.setAttack(target.getAttack() + 25);
                std::cout << Color::boldColor("  Parchemin de Feu ! +25 ATK !", Color::BRIGHT_RED) << "\n"; break;
        case 2: target.setDefense(target.getDefense() + 20);
                std::cout << Color::boldColor("  Parchemin de Glace ! +20 DEF !", Color::BRIGHT_CYAN) << "\n"; break;
        case 3: target.setSpeed(target.getSpeed() + 15);
                std::cout << Color::boldColor("  Parchemin de Foudre ! +15 VIT !", Color::BRIGHT_YELLOW) << "\n"; break;
        case 4: target.heal(30); target.setMaxHp(target.getMaxHp() + 30);
                std::cout << Color::boldColor("  Parchemin de Terre ! +30 PV max !", Color::BRIGHT_GREEN) << "\n"; break;
    }
}

void BerserkerPotion::use(Character& target) {
    target.setAttack(target.getAttack() + 50);
    target.setDefense(std::max(0, target.getDefense() - 20));
    std::cout << Color::boldColor("  BERSERKER ! +50 ATK, -20 DEF !", Color::BRIGHT_RED) << "\n";
}

void IronSkin::use(Character& target) {
    target.setDefense(target.getDefense() + 40);
    std::cout << Color::boldColor("  Peau de Fer ! +40 DEF !", Color::BRIGHT_BLUE) << "\n";
}
