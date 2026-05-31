#pragma once
#include <string>
#include <vector>
#include <memory>

class Skill;

class Character {
protected:
    std::string name;
    int hp, maxHp;
    int attack, defense, speed;
    int level;
    std::vector<std::shared_ptr<Skill>> skills;

public:
    Character(const std::string& name, int hp, int atk, int def, int spd);
    virtual ~Character() = default;

    virtual void useSkill(int index, Character& target) = 0;
    virtual std::string getClassName() const = 0;
    virtual void levelUp() = 0;

    // Résistance élémentaire (element passé comme int pour éviter dépendance circulaire)
    // 0=NONE, 1=FIRE, 2=ICE, 3=LIGHTNING, 4=EARTH
    // Retourne 1.0 par défaut (pas de résistance spéciale)
    virtual float getElementResistance(int element) const { return 1.0f; }
    virtual std::string getClassColor() const { return "\033[37m"; }

    void takeDamage(int dmg);
    void heal(int amount);
    bool isAlive() const;
    void displayStats() const;

    std::string getName() const { return name; }
    int getHp() const { return hp; }
    int getMaxHp() const { return maxHp; }
    int getAttack() const { return attack; }
    int getDefense() const { return defense; }
    int getSpeed() const { return speed; }
    int getLevel() const { return level; }
    const std::vector<std::shared_ptr<Skill>>& getSkills() const { return skills; }

    void setHp(int v) { hp = v; }
    void setMaxHp(int v) { maxHp = v; }
    void setAttack(int v) { attack = v; }
    void setDefense(int v) { defense = v; }
    void setSpeed(int v) { speed = v; }
    void setLevel(int v) { level = v; }
};
