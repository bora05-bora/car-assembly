#include "BoschBrakeRequiresBoschSteeringRule.h"
#include "../Car.h"
#include "../brake_systems/BoschBrakeSystem.h"
#include "../steering_systems/BoschSteeringSystem.h"

bool BoschBrakeRequiresBoschSteeringRule::isViolated(const Car& car) const {
    if (!car.getBrakeSystem() || !car.getSteeringSystem()) return false;
    return dynamic_cast<const BoschBrakeSystem*>(car.getBrakeSystem()) &&
           !dynamic_cast<const BoschSteeringSystem*>(car.getSteeringSystem());
}

std::string BoschBrakeRequiresBoschSteeringRule::getFailReason() const {
    return "Bosch 제동장치에는 Bosch 조향장치 이외 사용 불가";
}
