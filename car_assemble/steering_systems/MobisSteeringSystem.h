#pragma once
#include "../interfaces/ISteeringSystem.h"

class MobisSteeringSystem : public ISteeringSystem {
public:
    std::string getName() const override { return "Mobis"; }
};
