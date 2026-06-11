#pragma once
#include "interfaces/ICarType.h"
#include "interfaces/IEngine.h"
#include "interfaces/IBrakeSystem.h"
#include "interfaces/ISteeringSystem.h"

class Car {
    ICarType*        type_        = nullptr;
    IEngine*         engine_      = nullptr;
    IBrakeSystem*    brakeSystem_ = nullptr;
    ISteeringSystem* steering_    = nullptr;
public:
    void setType(ICarType* type);
    void setEngine(IEngine* engine);
    void setBrakeSystem(IBrakeSystem* brakeSystem);
    void setSteeringSystem(ISteeringSystem* steering);

    const ICarType*        getType()          const;
    const IEngine*         getEngine()        const;
    const IBrakeSystem*    getBrakeSystem()   const;
    const ISteeringSystem* getSteeringSystem()const;

    bool isFullyAssembled() const;
};
