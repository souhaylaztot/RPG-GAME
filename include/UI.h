#pragma once
#include "Color.h"
#include <string>
#include <vector>
#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm>

inline void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
inline void sleep_ms(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

// ─── Bars ─────────────────────────────────────────────────────────────────────
inline std::string dynamicHpBar(int cur, int max, int w = 20) {
    if (max <= 0) return "";
    float pct    = static_cast<float>(cur) / max;
    int   filled = std::max(0, std::min((int)(pct*w), w));
    std::string fc = pct > 0.6f ? Color::BRIGHT_GREEN
                   : pct > 0.3f ? Color::BRIGHT_YELLOW : Color::BRIGHT_RED;
    std::string bar = fc + "[";
    for (int i = 0; i < w; ++i)
        bar += i < filled ? "\xe2\x96\x88" : "\033[90m\xe2\x96\x91";
    bar += fc + "] " + Color::BOLD + fc + std::to_string((int)(pct*100)) + "%" + Color::RESET;
    if (pct <= 0.3f) bar += Color::BOLD + Color::BRIGHT_RED + " CRITIQUE!" + Color::RESET;
    else if (pct <= 0.6f) bar += Color::BRIGHT_YELLOW + " !" + Color::RESET;
    return bar;
}
inline std::string manaBar(int cur, int max, int w = 20) {
    if (max <= 0) return "";
    int f = std::max(0, std::min((cur*w)/max, w));
    std::string b = Color::BRIGHT_BLUE + "[";
    for (int i = 0; i < w; ++i) b += i < f ? "\xe2\x96\x88" : "\033[90m\xe2\x96\x91";
    return b + Color::BRIGHT_BLUE + "]" + Color::RESET;
}
inline std::string xpBar(int cur, int max, int w = 20) {
    if (max <= 0) return "";
    int f = std::max(0, std::min((cur*w)/max, w));
    std::string b = Color::BRIGHT_YELLOW + "[";
    for (int i = 0; i < w; ++i) b += i < f ? "\xe2\x96\x88" : "\033[90m\xe2\x96\x91";
    return b + Color::BRIGHT_YELLOW + "]" + Color::RESET;
}

// ─── Sprite ───────────────────────────────────────────────────────────────────
struct Sprite { std::vector<std::string> lines; std::string color; };

inline std::string spriteLine(std::initializer_list<std::string> parts) {
    std::string r; for (auto& p : parts) r += p; return r;
}
#define SL(...) spriteLine({__VA_ARGS__})

// ══════════════════════════════════════════════════════════════════════════════
//  HERO SPRITES  — clean stickman style with class identity
// ══════════════════════════════════════════════════════════════════════════════

// Warrior — heavy red plate, sword right hand, shield left
inline Sprite getWarriorSprite() {
    const std::string R="\033[91m",G="\033[93m",W="\033[97m",S="\033[37m",D="\033[90m";
    return {{
        SL("       ",G,"_/\\_"),
        SL("      ",R,"[",W,"##",R,"]"),
        SL("    ",W,"o-",R,"[",W,"##",R,"]-",W,"o"),
        SL("   ",S,"/",R," |",W,"##",R,"| ",G,"]="),
        SL("   ",S,"|",R," |",W,"##",R,"| ",G,"]"),
        SL("   ",S,"\\",R," |",W,"##",R,"| ",G,"]="),
        SL("     ",R,"/",W,"\\",R," /",W,"\\"),
        SL("    ",R,"/  ",W,"V",R,"  \\")
    }, Color::WARRIOR_PRIMARY};
}

// Mage — blue robes, tall hat, glowing orb staff
inline Sprite getMageSprite() {
    const std::string B="\033[94m",C="\033[96m",Y="\033[93m",W="\033[97m",P="\033[35m";
    return {{
        SL("      ",B,"/\\"),
        SL("     ",B,"/",C,"##",B,"\\"),
        SL("    ",W,"o",B,"|",C,"##",B,"|",W,"o"),
        SL("    ",B,"|",C,"##",B,"| ",Y,"*"),
        SL("    ",C,"\\",B,"##",C,"/ ",Y,"O"),
        SL("    ",B," /",C,"##",B,"\\ ",Y,"*"),
        SL("    ",B,"/    \\"),
        SL("   ",P,"~",B,"/    \\",P,"~")
    }, Color::MAGE_PRIMARY};
}

// Archer — green hood, bow drawn left, quiver right
inline Sprite getArcherSprite() {
    const std::string G="\033[92m",D="\033[32m",W="\033[97m",Y="\033[93m",Br="\033[33m";
    return {{
        SL("      ",D,"/\\"),
        SL("     ",G,"/",D,"##",G,"\\"),
        SL(Y,"<--",W,"o",G,"|",D,"##",G,"|",W,"o"),
        SL(Y," -->",G,"|",D,"##",G,"|"),
        SL("    ",G,"|",D,"##",G,"|",Br," |"),
        SL("    ",G,"/\\  /\\",Br," |"),
        SL("   ",D,"/  \\/  \\"),
        SL("   ",G,"        ")
    }, Color::ARCHER_PRIMARY};
}

// ══════════════════════════════════════════════════════════════════════════════
//  ENEMY SPRITES  — clean stickman style
// ══════════════════════════════════════════════════════════════════════════════

inline Sprite getGoblinSprite() {
    const std::string G="\033[32m",L="\033[92m",Y="\033[93m";
    return {{
        SL("    ",G,"/\\ /\\"),
        SL("    ",L,"|",G,"o",L," ",G,"o",L,"|"),
        SL("    ",G," \\__/ "),
        SL("   ",L," /##\\ "),
        SL("  ",Y,"/",L,"|##|",Y,"\\"),
        SL("   ",L," |##| "),
        SL("   ",G," /\\  /\\"),
        SL("   ",L,"/  \\/  \\")
    }, Color::ENEMY_PRIMARY};
}

inline Sprite getSkeletonSprite() {
    const std::string W="\033[97m",D="\033[37m",Y="\033[93m";
    return {{
        SL("    ",W," ___ "),
        SL("    ",W,"/",D,"X X",W,"\\"),
        SL("    ",D," \\-/ "),
        SL("   ",W," /||\\ "),
        SL("  ",Y,"/ ",W,"|||",Y," \\"),
        SL("    ",W," ||| "),
        SL("    ",D," | | "),
        SL("    ",W,"_| |_")
    }, Color::ENEMY_PRIMARY};
}

inline Sprite getWolfSprite() {
    const std::string W="\033[97m",D="\033[37m",R="\033[91m";
    return {{
        SL("   ",D," /\\ "),
        SL("  ",W,"/",D,"##",W,"\\"),
        SL("  ",D,"|",R,"o",D," ",R,"o",D,"|"),
        SL("  ",W," \\__/ "),
        SL(" ",D,"/######\\"),
        SL(" ",W,"|######|"),
        SL("  ",D,"|  |  |"),
        SL("  ",W,"/  /\\  \\")
    }, Color::ENEMY_PRIMARY};
}

inline Sprite getTrollSprite() {
    const std::string G="\033[32m",L="\033[92m",Br="\033[33m",R="\033[91m";
    return {{
        SL("  ",G," /\\  /\\"),
        SL("  ",L,"|",G,"##",L,"||",G,"##",L,"|"),
        SL("  ",G,"|",R,"o",G,"  ",R,"o",G,"|"),
        SL("  ",L,"  \\__/ "),
        SL(Br,"| ",L,"/####\\",Br," |"),
        SL(Br,"|",L,"|######|",Br,"|"),
        SL("  ",G," |    | "),
        SL("  ",L," /    \\ ")
    }, Color::ENEMY_PRIMARY};
}

inline Sprite getVampireSprite() {
    const std::string P="\033[35m",L="\033[95m",W="\033[97m",R="\033[91m";
    return {{
        SL("    ",P," /\\ "),
        SL("   ",L,"/",P,"##",L,"\\"),
        SL("   ",W,"o",L,"|",P,"##",L,"|",W,"o"),
        SL("   ",R," /\\ "),
        SL(P,"\\  ",L,"|",P,"##",L,"|",P,"  /"),
        SL(P," \\ ",L,"|",P,"##",L,"|",P," /"),
        SL("    ",P," /\\  /\\"),
        SL("    ",P,"/  \\/  \\")
    }, Color::ENEMY_PRIMARY};
}

inline Sprite getGolemSprite() {
    const std::string Y="\033[33m",L="\033[93m",G="\033[90m",W="\033[97m";
    return {{
        SL("  ",Y," /####\\"),
        SL("  ",L,"|",W,"*",L,"##",W,"*",L,"|"),
        SL("  ",Y,"|",G,"##",Y,"  ",G,"##",Y,"|"),
        SL("  ",L,"|########|"),
        SL("  ",Y," /######\\"),
        SL("  ",L,"|##########|"),
        SL("  ",Y,"  |    |  "),
        SL("  ",L," /|    |\\ ")
    }, Color::ENEMY_PRIMARY};
}

inline Sprite getSorcererSprite() {
    const std::string P="\033[35m",L="\033[95m",C="\033[96m",Y="\033[93m";
    return {{
        SL("    ",P," /\\ "),
        SL("   ",L,"/",P,"##",L,"\\"),
        SL("   ",P,"o",L,"|",P,"##",L,"|",P,"o"),
        SL("   ",L," \\##/ ",C,"O"),
        SL("   ",P," /##\\ ",Y,"*"),
        SL("  ",L,"/|####|\\ ",Y,"*"),
        SL("   ",P," |    |",Y,"*"),
        SL("   ",L," /    \\")
    }, Color::ENEMY_PRIMARY};
}

inline Sprite getDragonSprite() {
    const std::string R="\033[91m",D="\033[31m",Y="\033[93m",W="\033[97m";
    return {{
        SL(R,"/\\  ",D,"/######\\",R,"  /\\"),
        SL(D,"\\ \\ ",R,"|########|",D," / /"),
        SL("  ",R,"\\|",D,"|",W,"*",Y,"##",W,"*",D,"|",R,"|/"),
        SL("   ",D,"|##########|"),
        SL("   ",R," \\########/ "),
        SL("   ",D," /|######|\\ "),
        SL("  ",R,"/ |",D,"##  ##",R,"| \\"),
        SL("  ",D,"/  /      \\  \\")
    }, Color::ENEMY_PRIMARY};
}

// ══════════════════════════════════════════════════════════════════════════════
//  BOSS SPRITES  — unique crowns + color schemes
// ══════════════════════════════════════════════════════════════════════════════

// Floor 10 — Lich King: ice-blue crown with spikes
inline Sprite getLichKingSprite() {
    const std::string W="\033[97m",C="\033[96m",P="\033[95m",Y="\033[93m",D="\033[90m";
    return {{
        SL("   ",C,"*",Y,"|",C,"*",Y,"|",C,"*",Y,"|",C,"*"),   // spiked crown
        SL("   ",C,"[",W,"=====",C,"]"),
        SL("   ",C,"|",W,"* ",D,"##",W," *",C,"|"),
        SL("   ",W," \\",C,"###",W,"/ "),
        SL(P,"\\  ",C,"/|####|\\",P,"  /"),
        SL(P," \\ ",C,"/ |####| \\",P," /"),
        SL("    ",W,"  /\\    /\\ "),
        SL("    ",C," /  \\  /  \\")
    }, "\033[96m"};
}

// Floor 20 — Demon Lord: horned crown, dark red
inline Sprite getDemonLordSprite() {
    const std::string R="\033[91m",D="\033[31m",Y="\033[93m",W="\033[97m",K="\033[90m";
    return {{
        SL(R,"/\\",K,"___",R,"/\\",K,"___",R,"/\\"),  // horned crown
        SL(D," \\",R,"[",Y,"###",R,"]",D,"/ "),
        SL("  ",R,"|",W,"* ",D,"####",W," *",R,"|"),
        SL("  ",D," \\######/ "),
        SL(R,"/  ",D,"/|####|\\",R,"  \\"),
        SL(R,"|  ",D,"||####||",R,"  |"),
        SL("   ",D,"  /\\  /\\  "),
        SL("   ",R," /  \\/  \\ ")
    }, "\033[91m"};
}

// Floor 30 — Ancient Dragon: golden crown with gems
inline Sprite getAncientDragonSprite() {
    const std::string R="\033[91m",G="\033[93m",D="\033[31m",W="\033[97m",C="\033[96m";
    return {{
        SL(G," o",C,"*",G,"o",C,"*",G,"o",C,"*",G,"o"),  // gem crown
        SL(G,"[",W,"=======",G,"]"),
        SL("  ",R,"|",G,"* ",W,"########",G," *",R,"|"),
        SL("  ",D,"|############|"),
        SL("  ",R," \\##########/ "),
        SL(G,"/  ",R,"/|########|\\",G,"  \\"),
        SL(D,"/ / ",R,"|##    ##|",D," \\ \\"),
        SL(G,"/   ",D,"/        \\",G,"   \\")
    }, "\033[93m"};
}

// Floor 40 — Shadow God: dark void crown with stars
inline Sprite getShadowGodSprite() {
    const std::string B="\033[34m",L="\033[94m",W="\033[97m",C="\033[96m",S="\033[37m";
    return {{
        SL(S,".",B,"*",S,".",W,"*",S,".",B,"*",S,".",W,"*",S,"."),  // star crown
        SL(B,"[",L,"~~~~~",B,"]"),
        SL(B,"** ",W,"o",L,"|######|",W,"o",B," **"),
        SL("   ",L," \\####/   "),
        SL(B,"*  ",C,"/|######|\\",B,"  *"),
        SL(B,"* ",C,"/ |######| \\",B," *"),
        SL("    ",L,"  /\\    /\\  "),
        SL("    ",B," /  \\  /  \\ ")
    }, "\033[94m"};
}

// Optional boss — purple/silver crown
inline Sprite getOptionalBossSprite() {
    const std::string P="\033[35m",L="\033[95m",W="\033[97m",Y="\033[93m",C="\033[96m";
    return {{
        SL(Y," /",P,"\\",Y,"/",P,"\\",Y,"/"),   // jagged optional crown
        SL(P,"[",W,"~~~~~",P,"]"),
        SL(P,"|",W,"o ",C,"##",W," o",P,"|"),
        SL(P," \\",W,"###",P,"/ "),
        SL(L,"\\  ",P,"/|####|\\",L,"  /"),
        SL(L," \\ ",P,"/ |####| \\",L," /"),
        SL("    ",W,"  /\\    /\\ "),
        SL("    ",P," /  \\  /  \\")
    }, "\033[95m"};
}

inline Sprite getBossSprite(int floor) {
    if (floor >= 40) return getShadowGodSprite();
    if (floor >= 30) return getAncientDragonSprite();
    if (floor >= 20) return getDemonLordSprite();
    return getLichKingSprite();
}

inline Sprite getEnemySprite(const std::string& name) {
    if (name.find("Gobelin")   != std::string::npos) return getGoblinSprite();
    if (name.find("Squelette") != std::string::npos) return getSkeletonSprite();
    if (name.find("Dragon")    != std::string::npos) return getDragonSprite();
    if (name.find("Loup")      != std::string::npos) return getWolfSprite();
    if (name.find("Troll")     != std::string::npos) return getTrollSprite();
    if (name.find("Vampire")   != std::string::npos) return getVampireSprite();
    if (name.find("Golem")     != std::string::npos) return getGolemSprite();
    if (name.find("Sorcier")   != std::string::npos) return getSorcererSprite();
    if (name.find("Lich")      != std::string::npos) return getLichKingSprite();
    if (name.find("Demon")     != std::string::npos) return getDemonLordSprite();
    if (name.find("Ancient")   != std::string::npos) return getAncientDragonSprite();
    if (name.find("Ombre")     != std::string::npos) return getShadowGodSprite();
    // Optional bosses
    return getOptionalBossSprite();
}

inline Sprite getHeroSprite(const std::string& className) {
    if (className == "Guerrier") return getWarriorSprite();
    if (className == "Mage")     return getMageSprite();
    if (className == "Archer")   return getArcherSprite();
    return getWarriorSprite();
}

// ─── Screens ──────────────────────────────────────────────────────────────────
inline void armorRewardScreen(const std::string& name, const std::string& rarity,
                               const std::string& rc, const std::string& summary) {
    clearScreen();
    std::cout << "\n\n  " << Color::BOLD << Color::BRIGHT_YELLOW
              << "╔══════════════════════════════════════════╗\n"
              << "║       RECOMPENSE D'ARMURE !              ║\n"
              << "╚══════════════════════════════════════════╝\n" << Color::RESET << "\n";
    std::cout << "  " << Color::boldColor("Rarete : ", Color::BRIGHT_WHITE) << Color::boldColor(rarity, rc) << "\n";
    std::cout << "  " << Color::boldColor("Armure : ", Color::BRIGHT_WHITE) << Color::boldColor(name, rc) << "\n";
    std::cout << "  " << Color::boldColor("Bonus  : ", Color::BRIGHT_WHITE) << Color::colorize(summary, Color::BRIGHT_GREEN) << "\n\n";
    sleep_ms(400);
}

inline void bossIntroScreen(const std::string& bossName, int floor, const Sprite& sp) {
    clearScreen();
    std::cout << "\n  " << Color::BOLD << Color::BRIGHT_RED
              << "╔══════════════════════════════════════════════╗\n"
              << "║   BOSS  --  ETAGE " << floor
              << std::string(std::max(0, 26-(int)std::to_string(floor).size()), ' ') << "║\n"
              << "╚══════════════════════════════════════════════╝\n" << Color::RESET << "\n";
    for (auto& line : sp.lines) { std::cout << "        " << line << "\n"; sleep_ms(50); }
    std::cout << "\n  " << Color::boldColor(bossName, Color::BRIGHT_RED) << "\n\n";
    sleep_ms(700);
}

inline void animatedTitle() {
    clearScreen();
    std::vector<std::string> lines = {
        "  ╔══════════════════════════════════════════════════╗",
        "  ║                                                  ║",
        "  ║   ██████╗ ██████╗  ██████╗                      ║",
        "  ║   ██╔══██╗██╔══██╗██╔════╝                      ║",
        "  ║   ██████╔╝██████╔╝██║  ███╗                     ║",
        "  ║   ██╔══██╗██╔═══╝ ██║   ██║                     ║",
        "  ║   ██║  ██║██║     ╚██████╔╝                     ║",
        "  ║   ╚═╝  ╚═╝╚═╝      ╚═════╝   COMBAT RPG  ⚔     ║",
        "  ║                                                  ║",
        "  ╚══════════════════════════════════════════════════╝"
    };
    std::vector<std::string> colors = {
        Color::BRIGHT_YELLOW, Color::BRIGHT_YELLOW,
        Color::BRIGHT_RED,    Color::BRIGHT_RED,
        Color::BRIGHT_RED,    Color::BRIGHT_RED,
        Color::BRIGHT_RED,    Color::BRIGHT_WHITE,
        Color::BRIGHT_YELLOW, Color::BRIGHT_YELLOW
    };
    for (size_t i = 0; i < lines.size(); ++i) {
        std::cout << Color::BOLD << colors[i] << lines[i] << Color::RESET << "\n";
        sleep_ms(50);
    }
    std::cout << "\n";
}

inline void victoryScreen(const std::string& heroName, int floor, int xp) {
    clearScreen();
    std::cout << "\n\n  " << Color::BOLD << Color::BRIGHT_YELLOW
        << "╔══════════════════════════════════════╗\n"
        << "║   VICTOIRE !                         ║\n"
        << "╚══════════════════════════════════════╝\n" << Color::RESET << "\n";
    std::cout << "  " << Color::boldColor(heroName, Color::BRIGHT_WHITE)
              << Color::colorize(" a vaincu l'ennemi !", Color::BRIGHT_GREEN) << "\n";
    std::cout << "  " << Color::colorize("Etage : " + std::to_string(floor), Color::BRIGHT_CYAN) << "\n";
    std::cout << "  " << Color::colorize("XP : +" + std::to_string(xp), Color::BRIGHT_YELLOW) << "\n\n";
    sleep_ms(600);
}

inline void bossVictoryScreen(const std::string& heroName, const std::string& bossName, int floor) {
    clearScreen();
    std::cout << "\n\n  " << Color::BOLD << Color::BRIGHT_YELLOW
        << "╔══════════════════════════════════════════╗\n"
        << "║   BOSS VAINCU !                          ║\n"
        << "╚══════════════════════════════════════════╝\n" << Color::RESET << "\n";
    std::cout << "  " << Color::boldColor(heroName, Color::BRIGHT_WHITE)
              << Color::colorize(" a terrasse ", Color::BRIGHT_GREEN)
              << Color::boldColor(bossName, Color::BRIGHT_RED) << " !\n";
    std::cout << "  " << Color::colorize("Etage " + std::to_string(floor) + " complete !", Color::BRIGHT_CYAN) << "\n\n";
    sleep_ms(800);
}

inline void defeatScreen(const std::string& heroName, int floor) {
    clearScreen();
    std::cout << "\n\n  " << Color::BOLD << Color::BRIGHT_RED
        << "╔══════════════════════════════════════╗\n"
        << "║   GAME OVER                          ║\n"
        << "╚══════════════════════════════════════╝\n" << Color::RESET << "\n";
    std::cout << "  " << Color::boldColor(heroName, Color::BRIGHT_WHITE)
              << Color::colorize(" est tombe au combat...", Color::BRIGHT_RED) << "\n";
    std::cout << "  " << Color::colorize("Etage atteint : " + std::to_string(floor), Color::BRIGHT_YELLOW) << "\n\n";
    sleep_ms(700);
}
