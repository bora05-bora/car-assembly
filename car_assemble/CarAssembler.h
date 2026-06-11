#pragma once
#include "Car.h"
#include "CarUI.h"
#include "PartRegistry.h"
#include "CombinationValidator.h"

class CarAssembler {
    Car                   car_;
    PartRegistry&         registry_;
    CombinationValidator& validator_;
    CarUI                 ui_;
    int                   step_;
public:
    CarAssembler(PartRegistry& registry, CombinationValidator& validator);
    void run();
private:
    void showMenu() const;
    bool handleInput(int answer);
    void runCar()  const;
    void testCar() const;
};
