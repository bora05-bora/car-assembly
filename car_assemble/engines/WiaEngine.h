#pragma once
#include "../interfaces/IEngine.h"

class WiaEngine : public IEngine {
public:
    std::string getName() const override { return "WIA"; }
    bool isBroken() const override { return false; }
};
