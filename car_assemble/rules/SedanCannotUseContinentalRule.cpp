#include "SedanCannotUseContinentalRule.h"
#include "../Car.h"
#include "../car_types/Sedan.h"
#include "../brake_systems/ContinentalBrakeSystem.h"

bool SedanCannotUseContinentalRule::isViolated(const Car& car) const {
    if (!car.getType() || !car.getBrakeSystem()) return false;
    return dynamic_cast<const Sedan*>(car.getType()) &&
           dynamic_cast<const ContinentalBrakeSystem*>(car.getBrakeSystem());
}

std::string SedanCannotUseContinentalRule::getFailReason() const {
    return "Sedan에는 Continental 제동장치 사용 불가";
}
