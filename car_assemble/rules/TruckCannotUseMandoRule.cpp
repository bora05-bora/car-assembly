#include "TruckCannotUseMandoRule.h"
#include "../Car.h"
#include "../car_types/Truck.h"
#include "../brake_systems/MandoBrakeSystem.h"

bool TruckCannotUseMandoRule::isViolated(const Car& car) const {
    if (!car.getType() || !car.getBrakeSystem()) return false;
    return dynamic_cast<const Truck*>(car.getType()) &&
           dynamic_cast<const MandoBrakeSystem*>(car.getBrakeSystem());
}

std::string TruckCannotUseMandoRule::getFailReason() const {
    return "Truck에는 Mando 제동장치 사용 불가";
}
