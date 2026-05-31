#pragma once
#include "Hero.h"

class Warrior : public Hero {
public:
    Warrior(const std::string& name);
    std::string getClassName() const override { return "Guerrier"; }
    std::string getClassColor() const override { return "\033[91m"; }     // Rouge vif
    std::string getClassSecondary() const override { return "\033[31m"; } // Rouge sombre
    void levelUp() override;
};
