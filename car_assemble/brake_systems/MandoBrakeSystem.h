#pragma once
#include "../interfaces/IBrakeSystem.h"

class MandoBrakeSystem : public IBrakeSystem {
public:
    std::string getName() const override { return "Mando"; }
};
