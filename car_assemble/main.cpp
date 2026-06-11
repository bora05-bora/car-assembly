#ifdef _DEBUG

#include "gmock/gmock.h"

int main() {
    testing::InitGoogleMock();
    return RUN_ALL_TESTS();
}

#else

#include "CarAssembler.h"
#include "PartRegistry.h"
#include "CombinationValidator.h"
#include "car_types/Sedan.h"
#include "car_types/SUV.h"
#include "car_types/Truck.h"
#include "engines/GmEngine.h"
#include "engines/ToyotaEngine.h"
#include "engines/WiaEngine.h"
#include "engines/BrokenEngine.h"
#include "brake_systems/MandoBrakeSystem.h"
#include "brake_systems/ContinentalBrakeSystem.h"
#include "brake_systems/BoschBrakeSystem.h"
#include "steering_systems/BoschSteeringSystem.h"
#include "steering_systems/MobisSteeringSystem.h"
#include "rules/SedanCannotUseContinentalRule.h"
#include "rules/SuvCannotUseToyotaRule.h"
#include "rules/TruckCannotUseWiaRule.h"
#include "rules/TruckCannotUseMandoRule.h"
#include "rules/BoschBrakeRequiresBoschSteeringRule.h"

int main() {
    PartRegistry registry;
    registry.registerCarType(new Sedan());
    registry.registerCarType(new SUV());
    registry.registerCarType(new Truck());
    registry.registerEngine(new GmEngine());
    registry.registerEngine(new ToyotaEngine());
    registry.registerEngine(new WiaEngine());
    registry.registerEngine(new BrokenEngine());
    registry.registerBrakeSystem(new MandoBrakeSystem());
    registry.registerBrakeSystem(new ContinentalBrakeSystem());
    registry.registerBrakeSystem(new BoschBrakeSystem());
    registry.registerSteeringSystem(new BoschSteeringSystem());
    registry.registerSteeringSystem(new MobisSteeringSystem());

    CombinationValidator validator;
    validator.addRule(new SedanCannotUseContinentalRule());
    validator.addRule(new SuvCannotUseToyotaRule());
    validator.addRule(new TruckCannotUseWiaRule());
    validator.addRule(new TruckCannotUseMandoRule());
    validator.addRule(new BoschBrakeRequiresBoschSteeringRule());

    CarAssembler assembler(registry, validator);
    assembler.run();
}

#endif
