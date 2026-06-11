#pragma once
#include "../interfaces/IEngine.h"

class GmEngine : public IEngine {
public:
    std::string getName() const override { return "GM"; }
    bool isBroken() const override { return false; }
};
