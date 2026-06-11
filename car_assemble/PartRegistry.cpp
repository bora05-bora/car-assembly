#include "PartRegistry.h"
#include <cstdio>

void PartRegistry::registerCarType(ICarType* part)             { carTypes_.push_back(part); }
void PartRegistry::registerEngine(IEngine* part)               { engines_.push_back(part); }
void PartRegistry::registerBrakeSystem(IBrakeSystem* part)     { brakeSystems_.push_back(part); }
void PartRegistry::registerSteeringSystem(ISteeringSystem* part){ steeringSystems_.push_back(part); }

void PartRegistry::printCarTypes() const {
    for (int i = 0; i < (int)carTypes_.size(); i++)
        printf("%d. %s\n", i + 1, carTypes_[i]->getName().c_str());
}

void PartRegistry::printEngines() const {
    for (int i = 0; i < (int)engines_.size(); i++)
        printf("%d. %s\n", i + 1, engines_[i]->getName().c_str());
}

void PartRegistry::printBrakeSystems() const {
    for (int i = 0; i < (int)brakeSystems_.size(); i++)
        printf("%d. %s\n", i + 1, brakeSystems_[i]->getName().c_str());
}

void PartRegistry::printSteeringSystems() const {
    for (int i = 0; i < (int)steeringSystems_.size(); i++)
        printf("%d. %s\n", i + 1, steeringSystems_[i]->getName().c_str());
}

ICarType* PartRegistry::getCarType(int idx) const {
    if (idx < 1 || idx > (int)carTypes_.size()) return nullptr;
    return carTypes_[idx - 1];
}

IEngine* PartRegistry::getEngine(int idx) const {
    if (idx < 1 || idx > (int)engines_.size()) return nullptr;
    return engines_[idx - 1];
}

IBrakeSystem* PartRegistry::getBrakeSystem(int idx) const {
    if (idx < 1 || idx > (int)brakeSystems_.size()) return nullptr;
    return brakeSystems_[idx - 1];
}

ISteeringSystem* PartRegistry::getSteeringSystem(int idx) const {
    if (idx < 1 || idx > (int)steeringSystems_.size()) return nullptr;
    return steeringSystems_[idx - 1];
}

int PartRegistry::carTypeCount()        const { return (int)carTypes_.size(); }
int PartRegistry::engineCount()         const { return (int)engines_.size(); }
int PartRegistry::brakeSystemCount()    const { return (int)brakeSystems_.size(); }
int PartRegistry::steeringSystemCount() const { return (int)steeringSystems_.size(); }
