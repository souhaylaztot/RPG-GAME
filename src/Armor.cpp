#include "../include/Armor.h"
#include "../include/Color.h"
#include <cstdlib>
#include <array>

std::shared_ptr<Armor> Armor::generateRandom(int playerLevel) {
    static const std::array<std::string,4> headNames  = {"Casque","Heaume","Capuche","Couronne"};
    static const std::array<std::string,4> chestNames = {"Plastron","Cuirasse","Robe","Tunique"};
    static const std::array<std::string,4> legNames   = {"Jambières","Cuissardes","Pantalon","Braies"};
    static const std::array<std::string,4> bootNames  = {"Bottes","Sandales","Sabots","Sollerets"};
    static const std::array<std::string,5> prefixes   = {"Solide","Renforce","Ancien","Maudit","Forge"};
    static const std::array<std::string,5> enchants   = {"","Flamme","Givre","Foudre","Terre"};

    int rarityRoll = rand() % 100;
    std::string rarity, rarityColor;
    int rarityMult;
    if      (rarityRoll < 50) { rarity = "Commun";    rarityColor = Color::WHITE;          rarityMult = 10; }
    else if (rarityRoll < 75) { rarity = "Rare";       rarityColor = Color::BRIGHT_BLUE;    rarityMult = 16; }
    else if (rarityRoll < 90) { rarity = "Epique";     rarityColor = Color::BRIGHT_MAGENTA; rarityMult = 24; }
    else                      { rarity = "Legendaire"; rarityColor = Color::BRIGHT_YELLOW;  rarityMult = 38; }

    int slotRoll = rand() % 4;
    ArmorSlot slot = static_cast<ArmorSlot>(slotRoll);

    std::string baseName;
    switch (slot) {
        case ArmorSlot::HEAD:  baseName = headNames [rand()%4]; break;
        case ArmorSlot::CHEST: baseName = chestNames[rand()%4]; break;
        case ArmorSlot::LEGS:  baseName = legNames  [rand()%4]; break;
        case ArmorSlot::BOOTS: baseName = bootNames [rand()%4]; break;
        default: baseName = "Armure";
    }

    std::string enchant  = enchants[rand() % 5];
    std::string fullName = prefixes[rand()%5] + " " + baseName;
    if (!enchant.empty()) fullName += " de " + enchant;

    int s = playerLevel * rarityMult / 10;
    ArmorBuff buff;
    int elemType = 0;

    switch (slot) {
        case ArmorSlot::HEAD:
            buff.defBonus  = 2 + s + rand()%3;
            buff.manaBonus = 5 + s*2 + rand()%5;
            break;
        case ArmorSlot::CHEST:
            buff.hpBonus  = 10 + s*3 + rand()%10;
            buff.defBonus = 3  + s   + rand()%4;
            break;
        case ArmorSlot::LEGS:
            buff.defBonus = 2 + s + rand()%3;
            buff.spdBonus = 1 + s/2 + rand()%2;
            break;
        case ArmorSlot::BOOTS:
            buff.spdBonus = 2 + s + rand()%3;
            buff.atkBonus = 1 + s/2 + rand()%2;
            break;
        default: break;
    }

    // Elemental damage bonus for enchanted pieces
    if (!enchant.empty()) {
        if      (enchant == "Flamme") elemType = 1;
        else if (enchant == "Givre")  elemType = 2;
        else if (enchant == "Foudre") elemType = 3;
        else if (enchant == "Terre")  elemType = 4;
        buff.elemDmgBonus = 10 + rarityMult / 2;
        buff.elemType     = elemType;
    }

    if (rarity == "Legendaire") {
        buff.hpBonus   += 25; buff.atkBonus  += 8;
        buff.defBonus  += 8;  buff.spdBonus  += 4;
        buff.manaBonus += 20;
        if (buff.elemDmgBonus) buff.elemDmgBonus += 10;
    }

    buff.enchant = enchant;
    return std::make_shared<Armor>(fullName, slot, ArmorSet::NONE, buff, rarityColor, rarity);
}

std::shared_ptr<Armor> Armor::generateSetPiece(ArmorSet set, ArmorSlot slot, int playerLevel) {
    struct SetInfo { std::string name; std::string color; int elemType; };
    static const SetInfo setInfos[] = {
        {"",        Color::WHITE,          0},
        {"Dragon",  Color::BRIGHT_RED,     1},  // DRAGON
        {"Givre",   Color::BRIGHT_CYAN,    2},  // FROST
        {"Tonnerre",Color::BRIGHT_YELLOW,  3},  // THUNDER
        {"Ombre",   Color::BRIGHT_MAGENTA, 0},  // SHADOW
        {"Titan",   Color::BRIGHT_WHITE,   0},  // TITAN
        {"Fantome", Color::BRIGHT_BLUE,    0},  // PHANTOM
    };

    int idx = static_cast<int>(set);
    const SetInfo& si = setInfos[idx];

    static const std::string slotNames[] = {"Heaume","Plastron","Jambières","Bottes"};
    std::string fullName = si.name + " " + slotNames[static_cast<int>(slot)];

    int s = playerLevel;
    ArmorBuff buff;
    buff.elemType = si.elemType;

    switch (set) {
        case ArmorSet::DRAGON:
            buff.hpBonus = 15 + s*2; buff.atkBonus = 5 + s;
            buff.elemDmgBonus = 20; break;
        case ArmorSet::FROST:
            buff.defBonus = 10 + s*2; buff.manaBonus = 10 + s;
            buff.elemDmgBonus = 20; break;
        case ArmorSet::THUNDER:
            buff.spdBonus = 8 + s; buff.atkBonus = 4 + s;
            buff.elemDmgBonus = 20; break;
        case ArmorSet::SHADOW:
            buff.atkBonus = 8 + s; buff.spdBonus = 5 + s;
            buff.elemDmgBonus = 15; break;
        case ArmorSet::TITAN:
            buff.hpBonus = 30 + s*3; buff.defBonus = 12 + s*2;
            buff.atkBonus = 6 + s; break;
        case ArmorSet::PHANTOM:
            buff.spdBonus = 12 + s; buff.atkBonus = 7 + s;
            buff.manaBonus = 15 + s; break;
        default: break;
    }

    buff.enchant = si.name;
    return std::make_shared<Armor>(fullName, slot, set, buff, si.color, "Set");
}
