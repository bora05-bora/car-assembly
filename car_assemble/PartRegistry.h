#pragma once
#include <vector>
#include "interfaces/ICarType.h"
#include "interfaces/IEngine.h"
#include "interfaces/IBrakeSystem.h"
#include "interfaces/ISteeringSystem.h"

class PartRegistry {
    std::vector<ICarType*>        carTypes_;
    std::vector<IEngine*>         engines_;
    std::vector<IBrakeSystem*>    brakeSystems_;
    std::vector<ISteeringSystem*> steeringSystems_;
public:
    void registerCarType(ICarType* part);
    void registerEngine(IEngine* part);
    void registerBrakeSystem(IBrakeSystem* part);
    void registerSteeringSystem(ISteeringSystem* part);

    void printCarTypes()        const;
    void printEngines()         const;
    void printBrakeSystems()    const;
    void printSteeringSystems() const;

    ICarType*        getCarType(int idx)        const;
    IEngine*         getEngine(int idx)         const;
    IBrakeSystem*    getBrakeSystem(int idx)    const;
    ISteeringSystem* getSteeringSystem(int idx) const;

    int carTypeCount()        const;
    int engineCount()         const;
    int brakeSystemCount()    const;
    int steeringSystemCount() const;
};
