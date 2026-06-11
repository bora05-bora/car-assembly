#pragma once
#include "../interfaces/IEngine.h"

class BrokenEngine : public IEngine {
public:
    std::string getName() const override { return "Broken"; }
    bool isBroken() const override { return true; }
};
