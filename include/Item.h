#pragma once
#include <string>
#include <memory>

class Character;

// ─── Base Item ─────────────────────────────────────────────────────────────────
class Item {
protected:
    std::string name, description;
    int quantity;
public:
    Item(const std::string& n, const std::string& d, int q = 1)
        : name(n), description(d), quantity(q) {}
    virtual ~Item() = default;
    virtual void use(Character& target) = 0;
    virtual std::string getIcon() const { return "?"; }
    std::string getName()        const { return name; }
    std::string getDescription() const { return description; }
    int  getQuantity() const { return quantity; }
    bool consume() { if (quantity > 0) { --quantity; return true; } return false; }
};

// ─── Potions ───────────────────────────────────────────────────────────────────
class HealthPotion : public Item {
    int healAmount;
public:
    HealthPotion(int amount = 50, int qty = 1)
        : Item("Potion de Vie", "Restaure " + std::to_string(amount) + " PV", qty)
        , healAmount(amount) {}
    std::string getIcon() const override { return "[HP]"; }
    void use(Character& target) override;
};

class ManaPotion : public Item {
    int manaAmount;
public:
    ManaPotion(int amount = 30, int qty = 1)
        : Item("Potion de Mana", "Restaure " + std::to_string(amount) + " PM", qty)
        , manaAmount(amount) {}
    std::string getIcon() const override { return "[MP]"; }
    void use(Character& target) override;
};

class ElixirPotion : public Item {
public:
    ElixirPotion(int qty = 1)
        : Item("Elixir", "Restaure 100 PV et 50 PM", qty) {}
    std::string getIcon() const override { return "[EX]"; }
    void use(Character& target) override;
};

// ─── Rare Boss Drops ───────────────────────────────────────────────────────────
class PhoenixFeather : public Item {
public:
    PhoenixFeather(int qty = 1)
        : Item("Plume de Phenix", "Ressuscite avec 50% PV si mort (1 fois)", qty) {}
    std::string getIcon() const override { return "[PH]"; }
    void use(Character& target) override;
};

class DragonBlood : public Item {
public:
    DragonBlood(int qty = 1)
        : Item("Sang de Dragon", "+30 ATK pendant 3 tours", qty) {}
    std::string getIcon() const override { return "[DB]"; }
    void use(Character& target) override;
};

class SoulCrystal : public Item {
public:
    SoulCrystal(int qty = 1)
        : Item("Cristal d'Ame", "Restaure 200 PV et 100 PM", qty) {}
    std::string getIcon() const override { return "[SC]"; }
    void use(Character& target) override;
};

class AncientScroll : public Item {
    int element; // 1=Fire 2=Ice 3=Lightning 4=Earth
public:
    AncientScroll(int elem, int qty = 1);
    std::string getIcon() const override { return "[AS]"; }
    void use(Character& target) override;
};

class BerserkerPotion : public Item {
public:
    BerserkerPotion(int qty = 1)
        : Item("Potion Berserker", "+50 ATK, -20 DEF pendant 2 tours", qty) {}
    std::string getIcon() const override { return "[BK]"; }
    void use(Character& target) override;
};

class IronSkin : public Item {
public:
    IronSkin(int qty = 1)
        : Item("Peau de Fer", "+40 DEF pendant 3 tours", qty) {}
    std::string getIcon() const override { return "[IS]"; }
    void use(Character& target) override;
};
