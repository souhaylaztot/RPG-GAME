#include "../include/Skill.h"
#include "../include/Character.h"
#include "../include/Color.h"
#include <iostream>
#include <algorithm>

static std::string elementColor(int elem) {
    switch (elem) {
        case 1: return Color::FIRE_COLOR;
        case 2: return Color::ICE_COLOR;
        case 3: return Color::LIGHTNING_COLOR;
        case 4: return Color::EARTH_COLOR;
        default: return Color::WHITE;
    }
}

static std::string elementName(int elem) {
    switch (elem) {
        case 1: return "🔥 FEU";
        case 2: return "❄  GLACE";
        case 3: return "⚡ FOUDRE";
        case 4: return "🌿 TERRE";
        default: return "NEUTRE";
    }
}

void PhysicalSkill::execute(Character& user, Character& target) {
    int dmg = static_cast<int>(user.getAttack() * multiplier);
    std::cout << Color::boldColor(user.getName(), user.getClassColor())
              << Color::RESET << " utilise "
              << Color::boldColor(name, Color::BRIGHT_WHITE) << " !\n";
    target.takeDamage(dmg);
}

void MagicSkill::execute(Character& user, Character& target) {
    int elemInt = static_cast<int>(element);
    std::string eColor = elementColor(elemInt);
    std::string eName  = elementName(elemInt);

    std::cout << Color::boldColor(user.getName(), user.getClassColor())
              << Color::RESET << " lance "
              << Color::boldColor(name, eColor) << " "
              << Color::colorize("[" + eName + "]", eColor) << " !\n";

    float resistance = target.getElementResistance(elemInt);
    int dmg = std::max(1, static_cast<int>(baseDamage * resistance));

    // Afficher si vulnérable ou résistant
    if (resistance > 1.0f)
        std::cout << Color::boldColor("  >> VULNERABLE ! ", Color::BRIGHT_RED)
                  << "x" << resistance << "\n";
    else if (resistance < 1.0f)
        std::cout << Color::boldColor("  >> RESISTANT !  ", Color::BRIGHT_BLUE)
                  << "x" << resistance << "\n";

    std::cout << Color::colorize("  Degats magiques : ", eColor)
              << Color::boldColor(std::to_string(dmg), Color::BRIGHT_WHITE) << "\n";

    target.setHp(std::max(0, target.getHp() - dmg));
    std::cout << Color::boldColor(target.getName(), target.getClassColor())
              << Color::RESET << " perd "
              << Color::boldColor(std::to_string(dmg), Color::BRIGHT_RED)
              << " PV ! (PV: " << target.getHp() << "/" << target.getMaxHp() << ")\n";
}

void HealSkill::execute(Character& user, Character& target) {
    std::cout << Color::boldColor(user.getName(), user.getClassColor())
              << Color::RESET << " utilise "
              << Color::boldColor(name, Color::BRIGHT_GREEN) << " !\n";
    user.heal(healAmount);
}

void MultiHitSkill::execute(Character& user, Character& target) {
    std::cout << Color::boldColor(user.getName(), user.getClassColor())
              << Color::RESET << " utilise "
              << Color::boldColor(name, Color::BRIGHT_YELLOW)
              << " (" << hits << " coups) !\n";
    for (int i = 0; i < hits; ++i) {
        int dmg = static_cast<int>(user.getAttack() * multiplierPerHit);
        std::cout << Color::colorize("  Coup " + std::to_string(i + 1) + ": ", Color::YELLOW);
        target.takeDamage(dmg);
    }
}
