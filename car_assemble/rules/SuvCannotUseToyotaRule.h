#pragma once
#include "../interfaces/ICombinationRule.h"

class SuvCannotUseToyotaRule : public ICombinationRule {
public:
    bool isViolated(const Car& car) const override;
    std::string getFailReason() const override;
};
