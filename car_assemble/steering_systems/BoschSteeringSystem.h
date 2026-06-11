#pragma once
#include "../interfaces/ISteeringSystem.h"

class BoschSteeringSystem : public ISteeringSystem {
public:
    std::string getName() const override { return "Bosch"; }
};
