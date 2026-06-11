#pragma once
#include <vector>
#include <string>
#include "interfaces/ICombinationRule.h"

class Car;

class CombinationValidator {
    std::vector<ICombinationRule*> rules_;
public:
    void addRule(ICombinationRule* rule);

    bool        isValid(const Car& car) const;
    std::string getFailReason(const Car& car) const;
};
