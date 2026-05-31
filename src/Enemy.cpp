#include "../include/Enemy.h"
#include "../include/Skill.h"
#include "../include/Color.h"
#include <iostream>
#include <cstdlib>
#include <algorithm>

Enemy::Enemy(const std::string& n, int hp, int atk, int def, int spd, int xp, BossType bt)
    : Character(n, hp, atk, def, spd), xpReward(xp), bossType(bt) {
    for (int i = 0; i <= 4; ++i)
        resistances[static_cast<Element>(i)] = 1.0f;
}

float Enemy::getResistance(Element elem) const {
    auto it = resistances.find(elem);
    return it != resistances.end() ? it->second : 1.0f;
}

float Enemy::getElementResistance(int element) const {
    return getResistance(static_cast<Element>(element));
}

void Enemy::useSkill(int index, Character& target) {
    if (skills.empty()) {
        std::cout << Color::boldColor(name, getClassColor()) << Color::RESET << " attaque !\n";
        target.takeDamage(attack);
        return;
    }
    int i = index % static_cast<int>(skills.size());
    skills[i]->execute(*this, target);
}

Enemy Enemy::generateRandom(int playerLevel) {
    static const std::string names[]   = {"Gobelin","Squelette","Loup","Troll","Vampire","Golem","Sorcier"};
    static const std::string prefixes[]= {"Sauvage","Ancien","Maudit","Enrage","Sombre","Corrompu"};

    std::string eName = std::string(prefixes[rand()%6]) + " " + names[rand()%7];
    int s = playerLevel;
    Enemy e(eName, 40+s*15+rand()%20, 8+s*3+rand()%5, 3+s*2+rand()%3, 5+s+rand()%4, 20+s*10+rand()%15);
    e.skills.push_back(std::make_shared<PhysicalSkill>("Frappe",   1.2f, "Attaque physique"));
    e.skills.push_back(std::make_shared<PhysicalSkill>("Coup Dur", 1.6f, "Coup puissant"));
    int weak = 1+rand()%4, res = 1+rand()%4;
    e.setResistance(static_cast<Element>(weak), 1.5f);
    if (res != weak) e.setResistance(static_cast<Element>(res), 0.5f);
    return e;
}

Enemy Enemy::generateBoss(int floor, int playerLevel) {
    struct BD { std::string name; int hpM, atkM, defM; };
    static const BD bosses[] = {
        {"Lich King Morthis",    5, 4, 3},
        {"Demon Lord Kael'Zar",  7, 5, 4},
        {"Ancient Dragon Vreth", 9, 6, 5},
        {"Ombre Dieu Nyx",      12, 8, 6}
    };
    int tier = std::min((floor/10)-1, 3);
    const BD& bd = bosses[tier];
    int s = playerLevel;

    Enemy boss(bd.name, 150+s*bd.hpM*10, 15+s*bd.atkM, 8+s*bd.defM, 6+s, 200+floor*20, BossType::MANDATORY);
    boss.setLevel(floor/5);
    boss.skills.push_back(std::make_shared<PhysicalSkill>("Frappe Titanesque", 2.0f, "Coup devastateur"));
    boss.skills.push_back(std::make_shared<MagicSkill>("Malediction",   80, Element::FIRE,      0, "Magie de feu"));
    boss.skills.push_back(std::make_shared<MagicSkill>("Tempete Noire", 90, Element::LIGHTNING,  0, "Foudre noire"));
    boss.skills.push_back(std::make_shared<MultiHitSkill>("Assaut Frenetique", 3, 0.8f, "3 coups"));
    for (int i = 1; i <= 4; ++i)
        boss.setResistance(static_cast<Element>(i), 0.7f);
    return boss;
}

Enemy Enemy::generateOptionalBoss(int floor, int playerLevel) {
    struct OD { std::string name; int hpM, atkM, defM; };
    static const OD opts[] = {
        {"Spectre Errant",   3, 3, 2}, {"Chimere Sauvage",  4, 4, 2},
        {"Golem de Cristal", 4, 2, 5}, {"Vampire Ancien",   3, 4, 3},
        {"Hydre des Marais", 5, 3, 3}, {"Titan de Pierre",  4, 3, 5},
        {"Sorcier Maudit",   3, 5, 2}, {"Phenix Noir",      4, 4, 3}
    };
    const OD& od = opts[rand()%8];
    int s = playerLevel;

    Enemy boss(od.name, 80+s*od.hpM*8, 12+s*od.atkM, 5+s*od.defM, 8+s, 120+floor*15, BossType::OPTIONAL);
    boss.setLevel(floor/3);
    boss.skills.push_back(std::make_shared<PhysicalSkill>("Attaque Speciale", 1.8f, "Frappe puissante"));
    boss.skills.push_back(std::make_shared<MagicSkill>("Sort Elementaire", 60, static_cast<Element>(1+rand()%4), 0, "Magie"));
    boss.skills.push_back(std::make_shared<MultiHitSkill>("Rafale", 2, 0.9f, "Double frappe"));
    boss.setResistance(static_cast<Element>(1+rand()%4), 1.5f);
    return boss;
}
