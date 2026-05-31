#pragma once
#include <string>
#include <algorithm>

#ifdef _WIN32
#include <windows.h>
#endif

// ─── Codes ANSI ───────────────────────────────────────────────────────────────
namespace Color {
    const std::string RESET   = "\033[0m";
    const std::string BOLD    = "\033[1m";
    const std::string DIM     = "\033[2m";

    const std::string BLACK   = "\033[30m";
    const std::string RED     = "\033[31m";
    const std::string GREEN   = "\033[32m";
    const std::string YELLOW  = "\033[33m";
    const std::string BLUE    = "\033[34m";
    const std::string MAGENTA = "\033[35m";
    const std::string CYAN    = "\033[36m";
    const std::string WHITE   = "\033[37m";

    const std::string BRIGHT_RED     = "\033[91m";
    const std::string BRIGHT_GREEN   = "\033[92m";
    const std::string BRIGHT_YELLOW  = "\033[93m";
    const std::string BRIGHT_BLUE    = "\033[94m";
    const std::string BRIGHT_MAGENTA = "\033[95m";
    const std::string BRIGHT_CYAN    = "\033[96m";
    const std::string BRIGHT_WHITE   = "\033[97m";

    // Couleurs de fond
    const std::string BG_RED     = "\033[41m";
    const std::string BG_BLUE    = "\033[44m";
    const std::string BG_GREEN   = "\033[42m";
    const std::string BG_YELLOW  = "\033[43m";
    const std::string BG_MAGENTA = "\033[45m";
    const std::string BG_BLACK   = "\033[40m";

    // ── Schéma par classe ──────────────────────────────────────────────────────
    // Guerrier : Rouge vif  (force, sang)
    const std::string WARRIOR_PRIMARY   = "\033[91m";
    const std::string WARRIOR_SECONDARY = "\033[31m";
    const std::string WARRIOR_ACCENT    = "\033[93m";

    // Mage : Bleu vif  (magie, mystère)
    const std::string MAGE_PRIMARY   = "\033[94m";
    const std::string MAGE_SECONDARY = "\033[34m";
    const std::string MAGE_ACCENT    = "\033[96m";

    // Archer : Vert vif  (nature, forêt)
    const std::string ARCHER_PRIMARY   = "\033[92m";
    const std::string ARCHER_SECONDARY = "\033[32m";
    const std::string ARCHER_ACCENT    = "\033[93m";

    // Ennemi : Magenta  (danger, corruption)
    const std::string ENEMY_PRIMARY   = "\033[95m";
    const std::string ENEMY_SECONDARY = "\033[35m";
    const std::string ENEMY_ACCENT    = "\033[91m";

    // ── Schéma par élément ─────────────────────────────────────────────────────
    const std::string FIRE_COLOR      = "\033[91m";
    const std::string ICE_COLOR       = "\033[96m";
    const std::string LIGHTNING_COLOR = "\033[93m";
    const std::string EARTH_COLOR     = "\033[32m";
    const std::string NONE_COLOR      = "\033[37m";

    // ── Utilitaires ────────────────────────────────────────────────────────────
    inline std::string colorize(const std::string& text, const std::string& color) {
        return color + text + RESET;
    }
    inline std::string bold(const std::string& text) {
        return BOLD + text + RESET;
    }
    inline std::string boldColor(const std::string& text, const std::string& color) {
        return BOLD + color + text + RESET;
    }
}

// ─── Barre de progression colorée ─────────────────────────────────────────────
inline std::string makeBar(int current, int max, int width,
                           const std::string& fillColor) {
    if (max <= 0) return "";
    int filled = std::max(0, std::min((current * width) / max, width));
    std::string bar = fillColor + "[";
    for (int i = 0; i < width; ++i)
        bar += (i < filled) ? "\xe2\x96\x88" : "\033[90m\xe2\x96\x91" + fillColor;
    bar += Color::RESET + "]";
    return bar;
}

// ─── Activer ANSI sur Windows ──────────────────────────────────────────────────
inline void enableANSI() {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
}
