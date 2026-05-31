#pragma once
#include <string>
#include <memory>

enum class ArmorSlot { HEAD, CHEST, LEGS, BOOTS, NONE };

// Armor set names — wearing all 4 pieces of a set grants a bonus
enum class ArmorSet {
    NONE,
    DRAGON,      // Fire damage +20%, +50 HP
    FROST,       // Ice damage +20%, +30 DEF
    THUNDER,     // Lightning damage +20%, +20 SPD
    SHADOW,      // All damage +15%, +15% crit
    TITAN,       // +100 HP, +30 DEF, +20 ATK
    PHANTOM      // +40 SPD, +30 ATK, skills cost -5 mana
};

struct ArmorBuff {
    int hpBonus      = 0;
    int atkBonus     = 0;
    int defBonus     = 0;
    int spdBonus     = 0;
    int manaBonus    = 0;
    int elemDmgBonus = 0;   // % bonus to elemental damage
    int elemType     = 0;   // 0=none 1=fire 2=ice 3=lightning 4=earth
    std::string enchant;
};

class Armor {
public:
    std::string name;
    ArmorSlot   slot;
    ArmorSet    set;
    ArmorBuff   buff;
    std::string color;
    std::string rarity;  // Commun, Rare, Epique, Legendaire, Set

    Armor(const std::string& n, ArmorSlot s, ArmorSet as,
          const ArmorBuff& b, const std::string& col, const std::string& rar)
        : name(n), slot(s), set(as), buff(b), color(col), rarity(rar) {}

    std::string slotName() const {
        if (slot == ArmorSlot::HEAD)  return "Tete  ";
        if (slot == ArmorSlot::CHEST) return "Torse ";
        if (slot == ArmorSlot::LEGS)  return "Jambes";
        if (slot == ArmorSlot::BOOTS) return "Bottes";
        return "?";
    }

    std::string setName() const {
        switch (set) {
            case ArmorSet::DRAGON:  return "Dragon";
            case ArmorSet::FROST:   return "Givre";
            case ArmorSet::THUNDER: return "Tonnerre";
            case ArmorSet::SHADOW:  return "Ombre";
            case ArmorSet::TITAN:   return "Titan";
            case ArmorSet::PHANTOM: return "Fantome";
            default: return "";
        }
    }

    std::string elemName() const {
        switch (buff.elemType) {
            case 1: return "Feu";
            case 2: return "Glace";
            case 3: return "Foudre";
            case 4: return "Terre";
            default: return "";
        }
    }

    std::string summary() const {
        std::string s;
        if (buff.hpBonus)      s += " +" + std::to_string(buff.hpBonus)   + "PV";
        if (buff.atkBonus)     s += " +" + std::to_string(buff.atkBonus)  + "ATK";
        if (buff.defBonus)     s += " +" + std::to_string(buff.defBonus)  + "DEF";
        if (buff.spdBonus)     s += " +" + std::to_string(buff.spdBonus)  + "VIT";
        if (buff.manaBonus)    s += " +" + std::to_string(buff.manaBonus) + "MP";
        if (buff.elemDmgBonus) s += " +" + std::to_string(buff.elemDmgBonus) + "%" + elemName();
        if (!buff.enchant.empty()) s += " [" + buff.enchant + "]";
        if (set != ArmorSet::NONE) s += " {Set:" + setName() + "}";
        return s;
    }

    static std::shared_ptr<Armor> generateRandom(int playerLevel);
    static std::shared_ptr<Armor> generateSetPiece(ArmorSet set, ArmorSlot slot, int playerLevel);
};
