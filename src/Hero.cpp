#include "../include/Hero.h"
#include "../include/Skill.h"
#include "../include/Color.h"
#include "../include/UI.h"
#include <iostream>
#include <algorithm>

Hero::Hero(const std::string& name, int hp, int mana, int atk, int def, int spd)
    : Character(name, hp, atk, def, spd), mana(mana), maxMana(mana),
      xp(0), xpToNextLevel(100) {}

void Hero::useSkill(int index, Character& target) {
    if (index < 0 || index >= static_cast<int>(skills.size())) return;
    auto& skill = skills[index];
    if (mana < skill->getManaCost()) {
        std::cout << Color::boldColor("  Pas assez de mana ! ", Color::BRIGHT_RED)
                  << "(" << mana << "/" << skill->getManaCost() << ")\n";
        return;
    }
    mana -= skill->getManaCost();
    skill->execute(*this, target);
}

void Hero::gainXp(int amount) {
    xp += amount;
    std::cout << "\n  " << Color::boldColor(name, getClassColor())
              << Color::RESET << " gagne "
              << Color::boldColor("+" + std::to_string(amount) + " XP", Color::BRIGHT_YELLOW)
              << "\n  XP: " << xpBar(xp, xpToNextLevel, 22)
              << " " << xp << "/" << xpToNextLevel << "\n";
    while (xp >= xpToNextLevel) {
        xp -= xpToNextLevel;
        xpToNextLevel = static_cast<int>(xpToNextLevel * 1.5);
        levelUp();
    }
}

void Hero::levelUp() {
    level++;
    std::string c = getClassColor();
    std::cout << "\n  " << Color::BOLD << Color::BG_BLACK << c
              << "  ╔══════════════════════════════╗  \n"
              << "  ║  ★  NIVEAU SUPERIEUR !  ★   ║  \n"
              << "  ║  " << name << " -> Niveau " << level
              << std::string(std::max(0, 17 - (int)name.size()
                                          - (int)std::to_string(level).size()), ' ')
              << "║  \n"
              << "  ╚══════════════════════════════╝  "
              << Color::RESET << "\n";
    sleep_ms(300);
}

void Hero::useItem(int index) {
    if (index < 0 || index >= static_cast<int>(inventory.size())) return;
    auto& item = inventory[index];
    if (item->getQuantity() <= 0) {
        std::cout << Color::colorize("  Cet objet est epuise !\n", Color::BRIGHT_RED);
        return;
    }
    item->use(*this);
    item->consume();
}

void Hero::addItem(std::shared_ptr<Item> item) {
    inventory.push_back(item);
}

// ─── Equip armor ───────────────────────────────────────────────────────────────
void Hero::equipArmor(std::shared_ptr<Armor> armor) {
    // Remove old armor buffs if slot occupied
    auto it = equippedArmor.find(armor->slot);
    if (it != equippedArmor.end()) {
        auto& old = it->second->buff;
        maxHp   -= old.hpBonus;
        hp       = std::min(hp, maxHp);
        attack  -= old.atkBonus;
        defense -= old.defBonus;
        speed   -= old.spdBonus;
        maxMana -= old.manaBonus;
        mana     = std::min(mana, maxMana);
    }
    // Apply new armor buffs
    auto& b = armor->buff;
    maxHp   += b.hpBonus;
    hp      += b.hpBonus;
    attack  += b.atkBonus;
    defense += b.defBonus;
    speed   += b.spdBonus;
    maxMana += b.manaBonus;
    mana    += b.manaBonus;

    equippedArmor[armor->slot] = armor;

    std::cout << "\n  " << Color::boldColor("Armure equipee : ", Color::BRIGHT_WHITE)
              << Color::boldColor(armor->name, armor->color)
              << Color::colorize(armor->summary(), Color::BRIGHT_GREEN) << "\n";
}

bool Hero::hasArmorSlot(ArmorSlot slot) const {
    return equippedArmor.count(slot) > 0;
}

// ─── Display inventory ─────────────────────────────────────────────────────────
void Hero::displayInventory() const {
    std::string c = getClassColor();
    std::cout << "\n  " << Color::BOLD << c
              << "╔══════════════════════════════════════╗\n"
              << "  ║            INVENTAIRE                ║\n"
              << "  ╚══════════════════════════════════════╝"
              << Color::RESET << "\n";
    if (inventory.empty()) {
        std::cout << Color::colorize("  (vide)\n", Color::DIM);
    } else {
        for (int i = 0; i < static_cast<int>(inventory.size()); ++i) {
            std::string icon = "🧪";
            if (inventory[i]->getName().find("Vie")    != std::string::npos) icon = "❤ ";
            if (inventory[i]->getName().find("Elixir") != std::string::npos) icon = "✨";
            std::cout << "  " << Color::boldColor("  " + std::to_string(i+1) + ".", c)
                      << " " << icon << " "
                      << Color::boldColor(inventory[i]->getName(), Color::BRIGHT_WHITE)
                      << Color::colorize("  x" + std::to_string(inventory[i]->getQuantity()),
                                         Color::BRIGHT_YELLOW)
                      << Color::colorize("   " + inventory[i]->getDescription(), Color::DIM)
                      << "\n";
        }
    }
    displayEquipment();
}

// ─── Display equipment ─────────────────────────────────────────────────────────
void Hero::displayEquipment() const {
    std::string c = getClassColor();
    std::cout << "\n  " << Color::BOLD << c
              << "╔══════════════════════════════════════╗\n"
              << "  ║            EQUIPEMENT                ║\n"
              << "  ╚══════════════════════════════════════╝"
              << Color::RESET << "\n";

    static const ArmorSlot slots[] = {
        ArmorSlot::HEAD, ArmorSlot::CHEST, ArmorSlot::LEGS, ArmorSlot::BOOTS
    };
    static const std::string slotIcons[] = {"🪖 Tete  ", "🛡 Torse ", "🩲 Jambes", "👢 Bottes"};

    for (int i = 0; i < 4; ++i) {
        std::cout << "  " << Color::boldColor("  " + slotIcons[i] + " : ", c);
        auto it = equippedArmor.find(slots[i]);
        if (it != equippedArmor.end()) {
            auto& a = it->second;
            std::cout << Color::boldColor(a->name, a->color)
                      << Color::colorize(a->summary(), Color::BRIGHT_GREEN);
        } else {
            std::cout << Color::colorize("(vide)", Color::DIM);
        }
        std::cout << "\n";
    }
}

// ─── Full stats ────────────────────────────────────────────────────────────────
void Hero::displayFullStats() const {
    std::string c  = getClassColor();
    std::string c2 = getClassSecondary();

    std::cout << "\n  " << Color::BOLD << c
              << "╔══════════════════════════════════════════╗\n"
              << "  ║  " << c2 << name << "  " << c << "[" << c2 << getClassName() << c << "]"
              << std::string(std::max(0, 36 - (int)name.size() - (int)getClassName().size()), ' ')
              << "║\n"
              << "  ╠══════════════════════════════════════════╣\n"
              << Color::RESET;

    std::cout << "  " << c << "║" << Color::RESET
              << "  Niveau : " << Color::boldColor(std::to_string(level), Color::BRIGHT_WHITE) << "\n";

    auto row = [&](const std::string& lbl, const std::string& bar, const std::string& val) {
        std::cout << "  " << c << "║" << Color::RESET
                  << "  " << Color::boldColor(lbl, Color::BRIGHT_WHITE)
                  << bar << " " << val << "\n";
    };

    row("PV   : ", dynamicHpBar(hp, maxHp, 18),
        Color::BRIGHT_WHITE + std::to_string(hp) + "/" + std::to_string(maxHp) + Color::RESET);
    row("Mana : ", manaBar(mana, maxMana, 18),
        Color::BRIGHT_BLUE + std::to_string(mana) + "/" + std::to_string(maxMana) + Color::RESET);
    row("XP   : ", xpBar(xp, xpToNextLevel, 18),
        Color::BRIGHT_YELLOW + std::to_string(xp) + "/" + std::to_string(xpToNextLevel) + Color::RESET);

    std::cout << "  " << c << "║" << Color::RESET
              << "  ATK : " << Color::boldColor(std::to_string(attack),  Color::BRIGHT_RED)
              << "   DEF : " << Color::boldColor(std::to_string(defense), Color::BRIGHT_BLUE)
              << "   VIT : " << Color::boldColor(std::to_string(speed),   Color::BRIGHT_YELLOW) << "\n";

    std::cout << "  " << c
              << "╚══════════════════════════════════════════╝"
              << Color::RESET << "\n";

    displayEquipment();
}
