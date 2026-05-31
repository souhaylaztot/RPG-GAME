#pragma once
#include <string>

class Character;

// Éléments magiques (valeurs int pour compatibilité avec getElementResistance)
enum class Element { NONE = 0, FIRE = 1, ICE = 2, LIGHTNING = 3, EARTH = 4 };

class Skill {
protected:
    std::string name;
    int manaCost;
    std::string description;

public:
    Skill(const std::string& name, int manaCost, const std::string& desc)
        : name(name), manaCost(manaCost), description(desc) {}
    virtual ~Skill() = default;

    virtual void execute(Character& user, Character& target) = 0;
    std::string getName() const { return name; }
    std::string getDescription() const { return description; }
    int getManaCost() const { return manaCost; }
};

// Attaque physique
class PhysicalSkill : public Skill {
    float multiplier;
public:
    PhysicalSkill(const std::string& name, float mult, const std::string& desc)
        : Skill(name, 0, desc), multiplier(mult) {}
    void execute(Character& user, Character& target) override;
};

// Attaque magique élémentaire - utilise getElementResistance() polymorphique
class MagicSkill : public Skill {
    int baseDamage;
    Element element;
public:
    MagicSkill(const std::string& name, int dmg, Element elem, int mana, const std::string& desc)
        : Skill(name, mana, desc), baseDamage(dmg), element(elem) {}
    void execute(Character& user, Character& target) override;
    Element getElement() const { return element; }
};

// Soin
class HealSkill : public Skill {
    int healAmount;
public:
    HealSkill(const std::string& name, int amount, int mana, const std::string& desc)
        : Skill(name, mana, desc), healAmount(amount) {}
    void execute(Character& user, Character& target) override;
};

// Attaque multi-coups
class MultiHitSkill : public Skill {
    int hits;
    float multiplierPerHit;
public:
    MultiHitSkill(const std::string& name, int hits, float mult, const std::string& desc)
        : Skill(name, 0, desc), hits(hits), multiplierPerHit(mult) {}
    void execute(Character& user, Character& target) override;
};
