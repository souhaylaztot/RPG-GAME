#pragma once
#include "Hero.h"

class Mage : public Hero {
public:
    Mage(const std::string& name);
    std::string getClassName() const override { return "Mage"; }
    std::string getClassColor() const override { return "\033[94m"; }     // Bleu vif
    std::string getClassSecondary() const override { return "\033[96m"; } // Cyan
    void levelUp() override;
};
