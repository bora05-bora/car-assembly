#pragma once
#include "../interfaces/ICombinationRule.h"

class BoschBrakeRequiresBoschSteeringRule : public ICombinationRule {
public:
    bool isViolated(const Car& car) const override;
    std::string getFailReason() const override;
};
