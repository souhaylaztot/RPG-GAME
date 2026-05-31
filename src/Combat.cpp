#include "../include/Combat.h"
#include "../include/Skill.h"
#include "../include/Color.h"
#include "../include/UI.h"
#include <iostream>
#include <limits>
#include <cstdlib>
#include <vector>
#include <string>

Combat::Combat(Hero& hero, Enemy& enemy, int floor)
    : hero(hero), enemy(enemy), floor(floor) {}

// ─── Combat log ───────────────────────────────────────────────────────────────
static std::vector<std::string> cLog;
static void logAdd(const std::string& msg) {
    cLog.push_back(msg);
    if ((int)cLog.size() > 6) cLog.erase(cLog.begin());
}
static void logClear() { cLog.clear(); }

// ─── Draw full battle scene ───────────────────────────────────────────────────
// state: 0=idle  1=hero-attack  2=enemy-attack  3=hero-hit  4=enemy-hit
static void drawScene(const Hero& hero, const Enemy& enemy, int floor,
                      int state, const std::string& msg = "", int dmg = 0) {
    clearScreen();

    std::string hc = hero.getClassColor();
    std::string ec = enemy.getClassColor();
    bool isBoss    = enemy.isBoss();
    bool isOpt     = enemy.getBossType() == BossType::OPTIONAL;

    // ── Header ────────────────────────────────────────────────────────────────
    std::string hdrColor = isBoss ? (isOpt ? Color::BRIGHT_MAGENTA : Color::BRIGHT_RED)
                                  : Color::BRIGHT_YELLOW;
    std::string hdrTitle = isBoss ? (isOpt ? "BOSS OPTIONNEL" : "BOSS") : "COMBAT";
    std::cout << Color::BOLD << hdrColor
              << "  ╔══════════════════════════════════════════════════════════╗\n"
              << "  ║  " << hdrTitle << "  --  Etage " << floor
              << std::string(std::max(0, 43-(int)hdrTitle.size()-(int)std::to_string(floor).size()), ' ')
              << "║\n"
              << "  ╚══════════════════════════════════════════════════════════╝\n"
              << Color::RESET;

    // ── Sprites ───────────────────────────────────────────────────────────────
    Sprite hs = getHeroSprite(hero.getClassName());
    Sprite es = getEnemySprite(enemy.getName());

    // Lunge offsets
    std::string hPad = (state == 1) ? "      " : "  ";   // hero lunges right
    std::string ePad = (state == 2) ? "   "    : "        "; // enemy lunges left

    size_t rows = std::max(hs.lines.size(), es.lines.size());
    std::cout << "\n";
    for (size_t i = 0; i < rows; ++i) {
        std::string l = i < hs.lines.size() ? hs.lines[i] : "              ";
        std::string r = i < es.lines.size() ? es.lines[i] : "              ";

        // Mid-row flash effects
        if (i == rows/2) {
            if (state == 1) l = Color::BOLD + Color::BRIGHT_WHITE + l + Color::RESET;
            if (state == 2) r = Color::BOLD + Color::BRIGHT_WHITE + r + Color::RESET;
            if (state == 3) l = Color::BOLD + Color::BG_RED + Color::BRIGHT_WHITE + l + Color::RESET;
            if (state == 4) r = Color::BOLD + Color::BG_RED + Color::BRIGHT_WHITE + r + Color::RESET;
        }
        std::cout << hPad << l << ePad << r << "\n";
    }

    // ── Floating damage number ────────────────────────────────────────────────
    if (dmg > 0) {
        std::string dc = dmg >= 40 ? Color::BRIGHT_RED : Color::BRIGHT_YELLOW;
        std::string ds = Color::BOLD + dc + " -" + std::to_string(dmg) + "!" + Color::RESET;
        if (state == 3) std::cout << "  " << ds << "\n";
        else            std::cout << std::string(32, ' ') << ds << "\n";
    } else {
        std::cout << "\n";
    }

    // ── HP / Mana bars ────────────────────────────────────────────────────────
    std::cout << "\n";
    std::cout << "  " << hc << Color::BOLD << hero.getName()
              << " [" << hero.getClassName() << "]" << Color::RESET << "\n";
    std::cout << "  PV  : " << dynamicHpBar(hero.getHp(),   hero.getMaxHp(),   24)
              << " " << Color::BRIGHT_WHITE << hero.getHp() << "/" << hero.getMaxHp() << Color::RESET << "\n";
    std::cout << "  Mana: " << manaBar(hero.getMana(), hero.getMaxMana(), 24)
              << " " << Color::BRIGHT_BLUE << hero.getMana() << "/" << hero.getMaxMana() << Color::RESET << "\n";

    std::cout << Color::DIM << "  ──────────────────────────────────────────────────────────\n" << Color::RESET;

    std::cout << "  " << ec << Color::BOLD << enemy.getName()
              << " [" << enemy.getClassName() << "]" << Color::RESET << "\n";
    std::cout << "  PV  : " << dynamicHpBar(enemy.getHp(), enemy.getMaxHp(), 24)
              << " " << Color::BRIGHT_WHITE << enemy.getHp() << "/" << enemy.getMaxHp() << Color::RESET << "\n";

    std::cout << Color::DIM << "  ──────────────────────────────────────────────────────────\n" << Color::RESET;

    // ── Action message ────────────────────────────────────────────────────────
    if (!msg.empty())
        std::cout << "\n  " << Color::BOLD << Color::BRIGHT_WHITE << "  >> " << msg << Color::RESET << "\n";
    else
        std::cout << "\n";

    // ── Combat log ────────────────────────────────────────────────────────────
    std::cout << Color::DIM << "  ─── Journal ──────────────────────────────────────────────\n" << Color::RESET;
    for (auto& e : cLog) std::cout << "  " << e << "\n";
    for (int i = (int)cLog.size(); i < 6; ++i) std::cout << "\n";
}

// ─── 3-frame attack animation ─────────────────────────────────────────────────
static void animateAttack(const Hero& hero, const Enemy& enemy, int floor,
                          bool heroAttacks, const std::string& skillName, int dmg) {
    std::string ac = heroAttacks ? hero.getClassColor() : enemy.getClassColor();
    std::string tc = heroAttacks ? enemy.getClassColor() : hero.getClassColor();
    std::string attacker = heroAttacks ? hero.getName() : enemy.getName();
    std::string target   = heroAttacks ? enemy.getName() : hero.getName();

    std::string msg1 = Color::boldColor(attacker, ac) + Color::RESET
                     + " utilise " + Color::boldColor(skillName, Color::BRIGHT_WHITE) + " !";
    std::string msg2 = Color::boldColor(target, tc) + Color::RESET
                     + " subit " + Color::boldColor(std::to_string(dmg), Color::BRIGHT_RED) + " degats !";

    // Frame 1 — attacker lunges
    drawScene(hero, enemy, floor, heroAttacks ? 1 : 2, msg1, 0);
    sleep_ms(200);

    // Frame 2 — target flashes red + damage number
    drawScene(hero, enemy, floor, heroAttacks ? 4 : 3, msg2, dmg);
    sleep_ms(280);

    // Frame 3 — settle, updated bars
    drawScene(hero, enemy, floor, 0, msg2, 0);
    sleep_ms(150);
}

// ─── displayCombatStatus ──────────────────────────────────────────────────────
void Combat::displayCombatStatus() const {
    drawScene(hero, enemy, floor, 0);
}

// ─── Skill menu ───────────────────────────────────────────────────────────────
void Combat::displaySkillMenu() const {
    std::string c = hero.getClassColor();
    std::cout << "\n  " << Color::BOLD << c
              << "┌──────────────────────────────────────────┐\n"
              << "  │            COMPETENCES                   │\n"
              << "  └──────────────────────────────────────────┘" << Color::RESET << "\n";
    const auto& skills = hero.getSkills();
    for (int i = 0; i < (int)skills.size(); ++i) {
        bool ok = hero.getMana() >= skills[i]->getManaCost();
        std::string nc = ok ? c : Color::DIM;
        std::cout << Color::boldColor(ok ? "  >> " : "  XX ", nc)
                  << Color::boldColor(std::to_string(i+1) + ". " + skills[i]->getName(),
                                      ok ? Color::BRIGHT_WHITE : Color::DIM)
                  << Color::colorize("  [Mana:" + std::to_string(skills[i]->getManaCost()) + "]",
                                     ok ? Color::BRIGHT_BLUE : Color::DIM)
                  << Color::colorize("  " + skills[i]->getDescription(), Color::DIM) << "\n";
    }
}

// ─── Hero turn ────────────────────────────────────────────────────────────────
void Combat::heroTurn() {
    bool done = false;
    while (!done) {
        drawScene(hero, enemy, floor, 0);
        std::string c = hero.getClassColor();
        std::cout << "\n  " << Color::BOLD << c
                  << "┌──────────────────────────────────────────┐\n"
                  << "  │              ACTION                      │\n"
                  << "  └──────────────────────────────────────────┘" << Color::RESET << "\n";
        std::cout << "  " << Color::boldColor("  1.", c) << " Attaquer\n";
        std::cout << "  " << Color::boldColor("  2.", c) << " Objet\n";
        std::cout << "  " << Color::boldColor("  3.", c) << " Stats\n";
        std::cout << "\n  " << Color::boldColor("  > ", c);

        int ch; std::cin >> ch;
        if (std::cin.fail()) { std::cin.clear(); std::cin.ignore(9999,'\n'); continue; }

        if (ch == 1) {
            drawScene(hero, enemy, floor, 0);
            displaySkillMenu();
            std::cout << "\n  " << Color::boldColor("  Competence > ", c);
            int sc; std::cin >> sc;
            if (sc < 1 || sc > (int)hero.getSkills().size()) continue;
            auto& skill = hero.getSkills()[sc-1];
            if (hero.getMana() < skill->getManaCost()) {
                logAdd(Color::BRIGHT_RED + "Pas assez de mana !" + Color::RESET);
                continue;
            }
            std::string sName = skill->getName();
            int eBefore = enemy.getHp();
            hero.useSkill(sc-1, enemy);
            int dmg = std::max(0, eBefore - enemy.getHp());
            logAdd(Color::boldColor(hero.getName(), hero.getClassColor()) + Color::RESET
                   + " >> " + sName + " >> "
                   + Color::boldColor("-"+std::to_string(dmg), Color::BRIGHT_RED));
            animateAttack(hero, enemy, floor, true, sName, dmg);
            done = true;

        } else if (ch == 2) {
            drawScene(hero, enemy, floor, 0);
            hero.displayInventory();
            const auto& inv = hero.getInventory();
            if (inv.empty()) { std::cout << Color::colorize("  Vide !\n", Color::BRIGHT_RED); sleep_ms(500); continue; }
            std::cout << "\n  " << Color::boldColor("  Objet > ", c);
            int ic; std::cin >> ic;
            if (ic < 1 || ic > (int)inv.size()) continue;
            int hBefore = hero.getHp();
            hero.useItem(ic-1);
            int healed = hero.getHp() - hBefore;
            logAdd(Color::boldColor(hero.getName(), hero.getClassColor()) + Color::RESET
                   + " utilise " + inv[ic-1]->getName()
                   + (healed > 0 ? " (+" + std::to_string(healed) + " PV)" : ""));
            drawScene(hero, enemy, floor, 0, "Objet utilise !", 0);
            sleep_ms(400);
            done = true;

        } else if (ch == 3) {
            clearScreen(); hero.displayFullStats();
            std::cout << Color::colorize("\n  Entree...", Color::DIM);
            std::cin.ignore(9999,'\n'); std::cin.get();
        }
    }
}

// ─── Enemy turn ───────────────────────────────────────────────────────────────
void Combat::enemyTurn() {
    drawScene(hero, enemy, floor, 2, enemy.getName() + " se prepare...", 0);
    sleep_ms(300);

    int hBefore = hero.getHp();
    int idx = rand() % std::max(1, (int)enemy.getSkills().size());
    std::string sName = enemy.getSkills().empty() ? "Attaque" : enemy.getSkills()[idx]->getName();
    enemy.useSkill(idx, hero);
    int dmg = std::max(0, hBefore - hero.getHp());

    logAdd(Color::boldColor(enemy.getName(), enemy.getClassColor()) + Color::RESET
           + " >> " + sName + " >> "
           + Color::boldColor("-"+std::to_string(dmg), Color::BRIGHT_RED));
    animateAttack(hero, enemy, floor, false, sName, dmg);
}

// ─── Main combat loop ─────────────────────────────────────────────────────────
bool Combat::run() {
    logClear();
    std::string hc = hero.getClassColor();
    std::string ec = enemy.getClassColor();

    // Intro
    clearScreen();
    std::cout << "\n\n  " << Color::BOLD << Color::BRIGHT_YELLOW << "  NOUVEAU COMBAT !\n\n" << Color::RESET;
    std::cout << "  " << Color::boldColor(hero.getName(), hc) << "  VS  " << Color::boldColor(enemy.getName(), ec) << "\n\n";
    Sprite hs = getHeroSprite(hero.getClassName());
    Sprite es = getEnemySprite(enemy.getName());
    for (size_t i = 0; i < std::max(hs.lines.size(), es.lines.size()); ++i) {
        std::string l = i < hs.lines.size() ? hs.lines[i] : "            ";
        std::string r = i < es.lines.size() ? es.lines[i] : "            ";
        std::cout << "  " << l << "          " << r << "\n";
        sleep_ms(30);
    }
    bool heroFirst = hero.getSpeed() >= enemy.getSpeed();
    std::cout << "\n  " << Color::colorize(
        heroFirst ? hero.getName() + " commence !" : enemy.getName() + " commence !",
        Color::BRIGHT_YELLOW) << "\n";
    sleep_ms(700);

    while (hero.isAlive() && enemy.isAlive()) {
        if (heroFirst) {
            heroTurn();
            if (!enemy.isAlive()) break;
            enemyTurn();
        } else {
            enemyTurn();
            if (!hero.isAlive()) break;
            heroTurn();
        }
    }

    if (hero.isAlive()) {
        drawScene(hero, enemy, floor, 0, "VICTOIRE ! " + enemy.getName() + " est vaincu !", 0);
        sleep_ms(500);
        if (enemy.isBoss())
            bossVictoryScreen(hero.getName(), enemy.getName(), floor);
        else
            victoryScreen(hero.getName(), floor, enemy.getXpReward());
        return true;
    } else {
        drawScene(hero, enemy, floor, 0, "DEFAITE... " + hero.getName() + " est mort.", 0);
        sleep_ms(500);
        defeatScreen(hero.getName(), floor);
        return false;
    }
}
