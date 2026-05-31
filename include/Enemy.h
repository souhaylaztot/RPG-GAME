#pragma once
#include "Character.h"
#include "Skill.h"
#include <map>

enum class BossType { NONE, MANDATORY, OPTIONAL };

class Enemy : public Character {
    int      xpReward;
    BossType bossType;
    std::map<Element, float> resistances;

public:
    Enemy(const std::string& name, int hp, int atk, int def, int spd, int xp,
          BossType bt = BossType::NONE);
    virtual ~Enemy() = default;

    void useSkill(int index, Character& target) override;
    std::string getClassName() const override {
        if (bossType == BossType::MANDATORY) return "BOSS";
        if (bossType == BossType::OPTIONAL)  return "BOSS OPTIONNEL";
        return "Ennemi";
    }
    void levelUp() override {}

    void  setResistance(Element elem, float value) { resistances[elem] = value; }
    float getResistance(Element elem) const;
    float getElementResistance(int element) const override;

    std::string getClassColor() const override {
        if (bossType == BossType::MANDATORY) return "\033[91m";
        if (bossType == BossType::OPTIONAL)  return "\033[95m";
        return "\033[35m";
    }

    int      getXpReward() const { return xpReward; }
    bool     isBoss()      const { return bossType != BossType::NONE; }
    BossType getBossType() const { return bossType; }

    static Enemy generateRandom(int playerLevel);
    static Enemy generateBoss(int floor, int playerLevel);
    static Enemy generateOptionalBoss(int floor, int playerLevel);
};
