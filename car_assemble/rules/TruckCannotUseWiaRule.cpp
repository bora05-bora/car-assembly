#include "TruckCannotUseWiaRule.h"
#include "../Car.h"
#include "../car_types/Truck.h"
#include "../engines/WiaEngine.h"

bool TruckCannotUseWiaRule::isViolated(const Car& car) const {
    if (!car.getType() || !car.getEngine()) return false;
    return dynamic_cast<const Truck*>(car.getType()) &&
           dynamic_cast<const WiaEngine*>(car.getEngine());
}

std::string TruckCannotUseWiaRule::getFailReason() const {
    return "Truck에는 WIA 엔진 사용 불가";
}
