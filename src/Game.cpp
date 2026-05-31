#include "../include/Game.h"
#include "../include/Warrior.h"
#include "../include/Mage.h"
#include "../include/Archer.h"
#include "../include/Enemy.h"
#include "../include/Combat.h"
#include "../include/Item.h"
#include "../include/Armor.h"
#include "../include/Color.h"
#include "../include/UI.h"
#include <iostream>
#include <limits>
#include <map>

Game::Game() : floor(1), killCount(0) {}

// ─── Class card ────────────────────────────────────────────────────────────────
static void printClassCard(const std::string& color, const Sprite& sprite,
                           const std::string& num, const std::string& name,
                           const std::string& stats, const std::string& skills) {
    std::cout << "\n  " << Color::BOLD << color
              << "┌────────────────────────────────────────────┐\n"
              << "  │  " << num << ".  " << Color::BRIGHT_WHITE << name
              << color << std::string(std::max(0, 35-(int)name.size()), ' ') << "│\n"
              << "  ├────────────────────────────────────────────┤\n";
    std::vector<std::string> statLines = {"  Stats : "+stats, "  Skills: "+skills, ""};
    size_t rows = std::max(sprite.lines.size(), statLines.size());
    for (size_t i = 0; i < rows; ++i) {
        std::string sp = i < sprite.lines.size() ? sprite.lines[i] : "                ";
        std::string st = i < statLines.size()    ? statLines[i]    : "";
        std::cout << "  │  " << sp << "  " << Color::RESET << Color::DIM << st
                  << Color::BOLD << color
                  << std::string(std::max(0, 22-(int)st.size()), ' ') << "│\n";
    }
    std::cout << "  └────────────────────────────────────────────┘\n" << Color::RESET;
}

// ─── Create hero ───────────────────────────────────────────────────────────────
std::shared_ptr<Hero> Game::createHero() {
    animatedTitle();
    std::cout << Color::boldColor("  Entrez le nom de votre heros : ", Color::BRIGHT_WHITE);
    std::string name;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, name);
    if (name.empty()) name = "Heros";

    clearScreen(); animatedTitle();
    std::cout << "\n" << Color::boldColor("  Choisissez votre classe :\n", Color::BRIGHT_WHITE);
    printClassCard(Color::WARRIOR_PRIMARY, getWarriorSprite(), "1", "GUERRIER",
                   "PV:150  ATK:20  DEF:15  VIT:8",  "Frappe, Tourbillon, Second Souffle");
    printClassCard(Color::MAGE_PRIMARY,    getMageSprite(),    "2", "MAGE",
                   "PV:80   ATK:10  DEF:5   VIT:10", "Feu, Glace, Foudre, Soin");
    printClassCard(Color::ARCHER_PRIMARY,  getArcherSprite(),  "3", "ARCHER",
                   "PV:100  ATK:15  DEF:8   VIT:15", "Tir Precis, Pluie de Fleches, Terre");

    std::shared_ptr<Hero> hero;
    int choice;
    while (true) {
        std::cout << Color::boldColor("\n  Votre choix (1-3) : ", Color::BRIGHT_WHITE);
        std::cin >> choice;
        if (std::cin.fail()) { std::cin.clear(); std::cin.ignore(9999,'\n'); continue; }
        if (choice == 1) { hero = std::make_shared<Warrior>(name); break; }
        if (choice == 2) { hero = std::make_shared<Mage>(name);    break; }
        if (choice == 3) { hero = std::make_shared<Archer>(name);  break; }
    }

    hero->addItem(std::make_shared<HealthPotion>(50, 3));
    hero->addItem(std::make_shared<ManaPotion>(30, 2));
    hero->addItem(std::make_shared<ElixirPotion>(1));

    clearScreen();
    std::string c = hero->getClassColor();
    Sprite sp = getHeroSprite(hero->getClassName());
    std::cout << "\n  " << Color::BOLD << c << "★ " << hero->getName()
              << " le " << hero->getClassName() << " est pret ! ★" << Color::RESET << "\n\n";
    for (auto& line : sp.lines) std::cout << "  " << line << "\n";
    hero->displayFullStats();
    std::cout << Color::colorize("\n  Appuyez sur Entree...", Color::DIM);
    std::cin.ignore(9999,'\n'); std::cin.get();
    return hero;
}

// ─── Boss loot ─────────────────────────────────────────────────────────────────
void Game::giveBossLoot(bool isOptional) {
    clearScreen();
    std::string title = isOptional ? "BUTIN BOSS OPTIONNEL" : "BUTIN BOSS";
    std::cout << "\n  " << Color::BOLD << Color::BRIGHT_YELLOW
              << "╔══════════════════════════════════════════╗\n"
              << "║   " << title
              << std::string(std::max(0, 39-(int)title.size()), ' ') << "║\n"
              << "╚══════════════════════════════════════════╝\n" << Color::RESET << "\n";

    // Always drop a set piece for optional bosses
    if (isOptional) {
        static const ArmorSet sets[] = {
            ArmorSet::DRAGON, ArmorSet::FROST, ArmorSet::THUNDER,
            ArmorSet::SHADOW, ArmorSet::TITAN, ArmorSet::PHANTOM
        };
        ArmorSet chosenSet = sets[rand()%6];
        ArmorSlot chosenSlot = static_cast<ArmorSlot>(rand()%4);
        auto setArmor = Armor::generateSetPiece(chosenSet, chosenSlot, hero->getLevel());
        std::cout << "  " << Color::boldColor("Piece de Set : ", Color::BRIGHT_WHITE)
                  << Color::boldColor(setArmor->name, setArmor->color)
                  << Color::colorize(setArmor->summary(), Color::BRIGHT_GREEN) << "\n";
        std::cout << "\n  " << Color::boldColor("Equiper ? (o/n) : ", Color::BRIGHT_WHITE);
        char ch; std::cin >> ch;
        if (ch == 'o' || ch == 'O') hero->equipArmor(setArmor);
    }

    // Rare item drops
    int roll = rand() % 10;
    std::shared_ptr<Item> drop;
    if (roll < 2)      drop = std::make_shared<PhoenixFeather>(1);
    else if (roll < 4) drop = std::make_shared<DragonBlood>(1);
    else if (roll < 6) drop = std::make_shared<SoulCrystal>(1);
    else if (roll < 7) drop = std::make_shared<AncientScroll>(1+rand()%4, 1);
    else if (roll < 8) drop = std::make_shared<BerserkerPotion>(1);
    else if (roll < 9) drop = std::make_shared<IronSkin>(1);
    else               drop = std::make_shared<ElixirPotion>(2);

    hero->addItem(drop);
    std::cout << "  " << Color::boldColor("Objet rare : ", Color::BRIGHT_WHITE)
              << Color::boldColor(drop->getName(), Color::BRIGHT_YELLOW)
              << Color::colorize("  " + drop->getDescription(), Color::DIM) << "\n";

    // Mandatory boss also drops a random armor
    if (!isOptional) {
        auto armor = Armor::generateRandom(hero->getLevel());
        std::cout << "\n  " << Color::boldColor("Armure : ", Color::BRIGHT_WHITE)
                  << Color::boldColor(armor->name, armor->color)
                  << Color::colorize(armor->summary(), Color::BRIGHT_GREEN) << "\n";
        std::cout << "  " << Color::boldColor("Equiper ? (o/n) : ", Color::BRIGHT_WHITE);
        char ch; std::cin >> ch;
        if (ch == 'o' || ch == 'O') hero->equipArmor(armor);
    }

    sleep_ms(400);
}

// ─── Armor reward every 5 kills ────────────────────────────────────────────────
void Game::giveArmorReward() {
    auto armor = Armor::generateRandom(hero->getLevel());
    armorRewardScreen(armor->name, armor->rarity, armor->color, armor->summary());
    std::cout << "\n  " << Color::boldColor("Equiper ? (o/n) : ", Color::BRIGHT_WHITE);
    char ch; std::cin >> ch;
    if (ch == 'o' || ch == 'O') hero->equipArmor(armor);
    sleep_ms(300);
}

// ─── Check set bonus ───────────────────────────────────────────────────────────
void Game::checkSetBonus() {
    // Count equipped set pieces
    std::map<ArmorSet, int> setCounts;
    for (auto& [slot, armor] : hero->getEquippedArmor())
        if (armor->set != ArmorSet::NONE)
            setCounts[armor->set]++;

    for (auto& [set, count] : setCounts) {
        if (count == 4) {
            clearScreen();
            std::cout << "\n  " << Color::BOLD << Color::BRIGHT_YELLOW
                      << "╔══════════════════════════════════════╗\n"
                      << "║   ★ BONUS DE SET COMPLET ! ★        ║\n"
                      << "╚══════════════════════════════════════╝\n" << Color::RESET << "\n";
            // Apply set bonus
            switch (set) {
                case ArmorSet::DRAGON:
                    hero->setMaxHp(hero->getMaxHp()+50); hero->setHp(hero->getHp()+50);
                    std::cout << Color::boldColor("  Set Dragon : +50 PV max, +20% degats Feu !", Color::BRIGHT_RED) << "\n"; break;
                case ArmorSet::FROST:
                    hero->setDefense(hero->getDefense()+30);
                    std::cout << Color::boldColor("  Set Givre : +30 DEF, +20% degats Glace !", Color::BRIGHT_CYAN) << "\n"; break;
                case ArmorSet::THUNDER:
                    hero->setSpeed(hero->getSpeed()+20);
                    std::cout << Color::boldColor("  Set Tonnerre : +20 VIT, +20% degats Foudre !", Color::BRIGHT_YELLOW) << "\n"; break;
                case ArmorSet::SHADOW:
                    hero->setAttack(hero->getAttack()+15);
                    std::cout << Color::boldColor("  Set Ombre : +15 ATK, +15% tous degats !", Color::BRIGHT_MAGENTA) << "\n"; break;
                case ArmorSet::TITAN:
                    hero->setMaxHp(hero->getMaxHp()+100); hero->setHp(hero->getHp()+100);
                    hero->setDefense(hero->getDefense()+30); hero->setAttack(hero->getAttack()+20);
                    std::cout << Color::boldColor("  Set Titan : +100 PV, +30 DEF, +20 ATK !", Color::BRIGHT_WHITE) << "\n"; break;
                case ArmorSet::PHANTOM:
                    hero->setSpeed(hero->getSpeed()+40); hero->setAttack(hero->getAttack()+30);
                    std::cout << Color::boldColor("  Set Fantome : +40 VIT, +30 ATK !", Color::BRIGHT_BLUE) << "\n"; break;
                default: break;
            }
            sleep_ms(1200);
        }
    }
}

// ─── Play one floor ────────────────────────────────────────────────────────────
bool Game::playFloor() {
    clearScreen();
    bool isMandatoryBoss = (floor % 10 == 0);
    bool isOptionalBoss  = (!isMandatoryBoss && floor % 3 == 0 && floor > 1);

    // Floor banner
    if (isMandatoryBoss) {
        std::cout << "\n  " << Color::BOLD << Color::BRIGHT_RED
                  << "╔══════════════════════════════════════════╗\n"
                  << "║   BOSS OBLIGATOIRE  --  ETAGE " << floor
                  << std::string(std::max(0, 12-(int)std::to_string(floor).size()), ' ') << "║\n"
                  << "╚══════════════════════════════════════════╝\n" << Color::RESET << "\n";
    } else if (isOptionalBoss) {
        std::cout << "\n  " << Color::BOLD << Color::BRIGHT_MAGENTA
                  << "╔══════════════════════════════════════════╗\n"
                  << "║   BOSS OPTIONNEL  --  ETAGE " << floor
                  << std::string(std::max(0, 14-(int)std::to_string(floor).size()), ' ') << "║\n"
                  << "╚══════════════════════════════════════════╝\n" << Color::RESET << "\n";
        std::cout << "  " << Color::boldColor("Affronter le boss optionnel ? (o/n) : ", Color::BRIGHT_WHITE);
        char ch; std::cin >> ch;
        if (ch != 'o' && ch != 'O') {
            // Skip optional boss, fight normal enemy instead
            isOptionalBoss = false;
        }
    } else {
        std::cout << "\n  " << Color::BOLD << Color::BRIGHT_YELLOW
                  << "╔══════════════════════════════════════╗\n"
                  << "║          ETAGE  " << floor
                  << std::string(std::max(0, 22-(int)std::to_string(floor).size()), ' ') << "║\n"
                  << "╚══════════════════════════════════════╝\n" << Color::RESET << "\n";
    }
    sleep_ms(300);

    // Generate enemy
    Enemy enemy = isMandatoryBoss  ? Enemy::generateBoss(floor, hero->getLevel())
                : isOptionalBoss   ? Enemy::generateOptionalBoss(floor, hero->getLevel())
                                   : Enemy::generateRandom(hero->getLevel());

    Sprite es = getEnemySprite(enemy.getName());

    if (isMandatoryBoss || isOptionalBoss) {
        bossIntroScreen(enemy.getName(), floor, es);
    } else {
        std::cout << enemy.getClassColor() << Color::BOLD
                  << "  Un " << enemy.getName() << " apparait !" << Color::RESET << "\n\n";
        for (auto& line : es.lines) { std::cout << "  " << line << "\n"; sleep_ms(40); }
        std::cout << "\n  PV: " << dynamicHpBar(enemy.getHp(), enemy.getMaxHp(), 20)
                  << " " << enemy.getHp() << "/" << enemy.getMaxHp() << "\n";
        sleep_ms(500);
    }

    Combat combat(*hero, enemy, floor);
    bool won = combat.run();

    if (won) {
        killCount++;
        hero->gainXp(enemy.getXpReward());

        if (isMandatoryBoss || isOptionalBoss) {
            giveBossLoot(isOptionalBoss);
        } else {
            // Normal loot
            int loot = rand() % 5;
            if      (loot == 0) { hero->addItem(std::make_shared<HealthPotion>(50,1));
                                   std::cout << Color::colorize("  Potion de Vie !\n", Color::BRIGHT_GREEN); }
            else if (loot == 1) { hero->addItem(std::make_shared<ManaPotion>(30,1));
                                   std::cout << Color::colorize("  Potion de Mana !\n", Color::BRIGHT_BLUE); }
            else if (loot == 2) { hero->addItem(std::make_shared<ElixirPotion>(1));
                                   std::cout << Color::colorize("  Elixir !\n", Color::BRIGHT_MAGENTA); }
        }

        // Armor reward every 5 kills
        if (killCount % 5 == 0) {
            std::cout << "\n  " << Color::boldColor("★ Recompense d'armure ! (" + std::to_string(killCount) + " kills) ★", Color::BRIGHT_YELLOW) << "\n";
            sleep_ms(300);
            giveArmorReward();
        }

        checkSetBonus();
        floor++;
    }
    return won;
}

// ─── Main menu ─────────────────────────────────────────────────────────────────
void Game::showMainMenu() {
    clearScreen();
    std::string c = hero->getClassColor();
    Sprite sp = getHeroSprite(hero->getClassName());
    std::cout << "\n";
    for (auto& line : sp.lines) std::cout << "  " << line << "\n";

    std::cout << "\n  " << Color::BOLD << c << hero->getName()
              << " [" << hero->getClassName() << "]"
              << Color::colorize("  Niv." + std::to_string(hero->getLevel()), Color::BRIGHT_WHITE)
              << Color::RESET << "\n";
    std::cout << "  PV  : " << dynamicHpBar(hero->getHp(),   hero->getMaxHp(),   22)
              << " " << hero->getHp() << "/" << hero->getMaxHp() << "\n";
    std::cout << "  Mana: " << manaBar(hero->getMana(), hero->getMaxMana(), 22)
              << " " << hero->getMana() << "/" << hero->getMaxMana() << "\n";
    std::cout << "  XP  : " << xpBar(hero->getXp(), hero->getXpToNextLevel(), 22)
              << " " << hero->getXp() << "/" << hero->getXpToNextLevel() << "\n";
    std::cout << "  Kills: " << Color::boldColor(std::to_string(killCount), Color::BRIGHT_YELLOW)
              << Color::colorize("  (armure dans " + std::to_string(5 - killCount%5) + " kills)", Color::DIM) << "\n";

    // Next floor indicator
    bool nextMandatory = ((floor) % 10 == 0);
    bool nextOptional  = (!nextMandatory && (floor) % 3 == 0 && floor > 1);
    if (nextMandatory)
        std::cout << "\n  " << Color::BOLD << Color::BRIGHT_RED << "  ☠  BOSS OBLIGATOIRE au prochain etage !" << Color::RESET << "\n";
    else if (nextOptional)
        std::cout << "\n  " << Color::BOLD << Color::BRIGHT_MAGENTA << "  ★  BOSS OPTIONNEL disponible !" << Color::RESET << "\n";

    std::cout << "\n  " << Color::BOLD << c
              << "┌──────────────────────────────────────────┐\n"
              << "  │              MENU PRINCIPAL              │\n"
              << "  └──────────────────────────────────────────┘" << Color::RESET << "\n";
    std::cout << "  " << Color::boldColor("  1.", c) << " Continuer  "
              << Color::colorize("[Etage " + std::to_string(floor) + "]",
                                 nextMandatory ? Color::BRIGHT_RED : nextOptional ? Color::BRIGHT_MAGENTA : Color::BRIGHT_YELLOW) << "\n";
    std::cout << "  " << Color::boldColor("  2.", c) << " Stats\n";
    std::cout << "  " << Color::boldColor("  3.", c) << " Inventaire & Equipement\n";
    std::cout << "  " << Color::boldColor("  4.", c) << " Quitter\n";
    std::cout << "\n  " << Color::boldColor("  > ", c);
}

// ─── Main loop ─────────────────────────────────────────────────────────────────
void Game::run() {
    enableANSI();
    srand(static_cast<unsigned>(time(nullptr)));
    hero = createHero();

    bool playing = true;
    while (playing && hero->isAlive()) {
        showMainMenu();
        int choice; std::cin >> choice;
        if (std::cin.fail()) { std::cin.clear(); std::cin.ignore(9999,'\n'); continue; }
        if (choice == 1) {
            playing = playFloor();
        } else if (choice == 2) {
            clearScreen(); hero->displayFullStats();
            std::cout << Color::colorize("\n  Appuyez sur Entree...", Color::DIM);
            std::cin.ignore(9999,'\n'); std::cin.get();
        } else if (choice == 3) {
            clearScreen(); hero->displayInventory();
            std::cout << Color::colorize("\n  Appuyez sur Entree...", Color::DIM);
            std::cin.ignore(9999,'\n'); std::cin.get();
        } else if (choice == 4) {
            std::cout << Color::colorize("\n  Au revoir ! Etage : " + std::to_string(floor) + "\n", Color::BRIGHT_YELLOW);
            playing = false;
        }
    }
    if (!hero->isAlive()) defeatScreen(hero->getName(), floor);
}
