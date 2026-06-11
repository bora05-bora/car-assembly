#pragma once
#include <string>

class Car;

class ICombinationRule {
public:
    virtual ~ICombinationRule() = default;
    virtual bool isViolated(const Car& car) const = 0;
    virtual std::string getFailReason() const = 0;
};
