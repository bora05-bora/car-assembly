#pragma once
#include "../interfaces/IEngine.h"

class ToyotaEngine : public IEngine {
public:
    std::string getName() const override { return "TOYOTA"; }
    bool isBroken() const override { return false; }
};
