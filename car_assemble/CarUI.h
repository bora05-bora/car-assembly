#pragma once
#include <string>

class PartRegistry;
class Car;

class CarUI {
public:
    void printCarTypeMenu(const PartRegistry& registry) const;
    void printEngineMenu(const PartRegistry& registry) const;
    void printBrakeMenu(const PartRegistry& registry) const;
    void printSteeringMenu(const PartRegistry& registry) const;
    void printRunResult(const Car& car) const;
    void printTestResult(bool isValid, const std::string& reason) const;
    int  getInput() const;
};
