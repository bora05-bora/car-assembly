#pragma once
#include "../interfaces/ICombinationRule.h"

class TruckCannotUseWiaRule : public ICombinationRule {
public:
    bool isViolated(const Car& car) const override;
    std::string getFailReason() const override;
};
