#include "CombinationValidator.h"
#include "Car.h"

void CombinationValidator::addRule(ICombinationRule* rule) {
    rules_.push_back(rule);
}

bool CombinationValidator::isValid(const Car& car) const {
    for (ICombinationRule* rule : rules_) {
        if (rule->isViolated(car)) return false;
    }
    return true;
}

std::string CombinationValidator::getFailReason(const Car& car) const {
    for (ICombinationRule* rule : rules_) {
        if (rule->isViolated(car)) return rule->getFailReason();
    }
    return "";
}
