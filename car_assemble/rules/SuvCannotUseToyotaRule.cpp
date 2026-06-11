#include "SuvCannotUseToyotaRule.h"
#include "../Car.h"
#include "../car_types/SUV.h"
#include "../engines/ToyotaEngine.h"

bool SuvCannotUseToyotaRule::isViolated(const Car& car) const {
    if (!car.getType() || !car.getEngine()) return false;
    return dynamic_cast<const SUV*>(car.getType()) &&
           dynamic_cast<const ToyotaEngine*>(car.getEngine());
}

std::string SuvCannotUseToyotaRule::getFailReason() const {
    return "SUV에는 TOYOTA 엔진 사용 불가";
}
