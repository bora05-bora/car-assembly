#ifdef _DEBUG

#include "gmock/gmock.h"

#include "../car_types/Sedan.h"
#include "../car_types/SUV.h"
#include "../car_types/Truck.h"
#include "../engines/GmEngine.h"
#include "../engines/ToyotaEngine.h"
#include "../engines/WiaEngine.h"
#include "../engines/BrokenEngine.h"
#include "../brake_systems/MandoBrakeSystem.h"
#include "../brake_systems/ContinentalBrakeSystem.h"
#include "../brake_systems/BoschBrakeSystem.h"
#include "../steering_systems/BoschSteeringSystem.h"
#include "../steering_systems/MobisSteeringSystem.h"

// CarType

TEST(CarTypeTest, Sedan_GetName) {
    Sedan sedan;
    EXPECT_EQ(sedan.getName(), "Sedan");
}

TEST(CarTypeTest, SUV_GetName) {
    SUV suv;
    EXPECT_EQ(suv.getName(), "SUV");
}

TEST(CarTypeTest, Truck_GetName) {
    Truck truck;
    EXPECT_EQ(truck.getName(), "Truck");
}

// Engine

TEST(EngineTest, GmEngine_GetName) {
    GmEngine engine;
    EXPECT_EQ(engine.getName(), "GM");
}

TEST(EngineTest, GmEngine_IsNotBroken) {
    GmEngine engine;
    EXPECT_FALSE(engine.isBroken());
}

TEST(EngineTest, ToyotaEngine_GetName) {
    ToyotaEngine engine;
    EXPECT_EQ(engine.getName(), "TOYOTA");
}

TEST(EngineTest, ToyotaEngine_IsNotBroken) {
    ToyotaEngine engine;
    EXPECT_FALSE(engine.isBroken());
}

TEST(EngineTest, WiaEngine_GetName) {
    WiaEngine engine;
    EXPECT_EQ(engine.getName(), "WIA");
}

TEST(EngineTest, WiaEngine_IsNotBroken) {
    WiaEngine engine;
    EXPECT_FALSE(engine.isBroken());
}

TEST(EngineTest, BrokenEngine_IsBroken) {
    BrokenEngine engine;
    EXPECT_TRUE(engine.isBroken());
}

// BrakeSystem

TEST(BrakeSystemTest, Mando_GetName) {
    MandoBrakeSystem brake;
    EXPECT_EQ(brake.getName(), "Mando");
}

TEST(BrakeSystemTest, Continental_GetName) {
    ContinentalBrakeSystem brake;
    EXPECT_EQ(brake.getName(), "Continental");
}

TEST(BrakeSystemTest, Bosch_GetName) {
    BoschBrakeSystem brake;
    EXPECT_EQ(brake.getName(), "Bosch");
}

// SteeringSystem

TEST(SteeringSystemTest, Bosch_GetName) {
    BoschSteeringSystem steering;
    EXPECT_EQ(steering.getName(), "Bosch");
}

TEST(SteeringSystemTest, Mobis_GetName) {
    MobisSteeringSystem steering;
    EXPECT_EQ(steering.getName(), "Mobis");
}

#endif
