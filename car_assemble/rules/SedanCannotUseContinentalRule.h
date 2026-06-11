#pragma once
#include "../interfaces/ICombinationRule.h"

class SedanCannotUseContinentalRule : public ICombinationRule {
public:
    bool isViolated(const Car& car) const override;
    std::string getFailReason() const override;
};
