#pragma once
#include "Character.h"
#include "Item.h"
#include "Armor.h"
#include <vector>
#include <memory>
#include <map>

class Hero : public Character {
protected:
    int mana, maxMana;
    int xp, xpToNextLevel;
    std::vector<std::shared_ptr<Item>>  inventory;
    std::map<ArmorSlot, std::shared_ptr<Armor>> equippedArmor;

public:
    Hero(const std::string& name, int hp, int mana, int atk, int def, int spd);
    virtual ~Hero() = default;

    void useSkill(int index, Character& target) override;
    void levelUp() override;
    void gainXp(int amount);
    void useItem(int index);
    void addItem(std::shared_ptr<Item> item);
    void displayInventory() const;
    void displayFullStats() const;

    // Armor
    void equipArmor(std::shared_ptr<Armor> armor);
    void displayEquipment() const;
    bool hasArmorSlot(ArmorSlot slot) const;

    virtual std::string getClassColor()     const override { return "\033[37m"; }
    virtual std::string getClassSecondary() const          { return "\033[37m"; }

    int getMana()          const { return mana; }
    int getMaxMana()       const { return maxMana; }
    int getXp()            const { return xp; }
    int getXpToNextLevel() const { return xpToNextLevel; }
    const std::vector<std::shared_ptr<Item>>&              getInventory()     const { return inventory; }
    const std::map<ArmorSlot, std::shared_ptr<Armor>>&     getEquippedArmor() const { return equippedArmor; }

    void setMana(int v)    { mana    = v; }
    void setMaxMana(int v) { maxMana = v; }
};
