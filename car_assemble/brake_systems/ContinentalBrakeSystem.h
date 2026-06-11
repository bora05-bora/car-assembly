#pragma once
#include "../interfaces/IBrakeSystem.h"

class ContinentalBrakeSystem : public IBrakeSystem {
public:
    std::string getName() const override { return "Continental"; }
};
