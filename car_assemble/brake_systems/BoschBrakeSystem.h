#pragma once
#include "../interfaces/IBrakeSystem.h"

class BoschBrakeSystem : public IBrakeSystem {
public:
    std::string getName() const override { return "Bosch"; }
};
