#pragma once
#include "Hero.h"

class Archer : public Hero {
public:
    Archer(const std::string& name);
    std::string getClassName() const override { return "Archer"; }
    std::string getClassColor() const override { return "\033[92m"; }     // Vert vif
    std::string getClassSecondary() const override { return "\033[32m"; } // Vert sombre
    void levelUp() override;
};
