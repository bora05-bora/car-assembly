#include "Car.h"

void Car::setType(ICarType* type)                   { type_        = type; }
void Car::setEngine(IEngine* engine)                { engine_      = engine; }
void Car::setBrakeSystem(IBrakeSystem* brakeSystem) { brakeSystem_ = brakeSystem; }
void Car::setSteeringSystem(ISteeringSystem* steering) { steering_ = steering; }

const ICarType*        Car::getType()          const { return type_; }
const IEngine*         Car::getEngine()        const { return engine_; }
const IBrakeSystem*    Car::getBrakeSystem()   const { return brakeSystem_; }
const ISteeringSystem* Car::getSteeringSystem()const { return steering_; }

bool Car::isFullyAssembled() const {
    return type_ && engine_ && brakeSystem_ && steering_;
}
