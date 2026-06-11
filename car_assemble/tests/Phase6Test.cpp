#ifdef _DEBUG

#include "gmock/gmock.h"

#include "../PartRegistry.h"
#include "../car_types/Sedan.h"
#include "../car_types/SUV.h"
#include "../car_types/Truck.h"
#include "../engines/GmEngine.h"
#include "../engines/BrokenEngine.h"
#include "../brake_systems/MandoBrakeSystem.h"
#include "../brake_systems/BoschBrakeSystem.h"
#include "../steering_systems/BoschSteeringSystem.h"
#include "../steering_systems/MobisSteeringSystem.h"

// register 후 count

TEST(PartRegistryTest, CarTypeCount_AfterRegister) {
    PartRegistry reg;
    reg.registerCarType(new Sedan());
    reg.registerCarType(new SUV());
    EXPECT_EQ(reg.carTypeCount(), 2);
}

TEST(PartRegistryTest, EngineCount_AfterRegister) {
    PartRegistry reg;
    reg.registerEngine(new GmEngine());
    EXPECT_EQ(reg.engineCount(), 1);
}

TEST(PartRegistryTest, BrakeSystemCount_AfterRegister) {
    PartRegistry reg;
    reg.registerBrakeSystem(new MandoBrakeSystem());
    reg.registerBrakeSystem(new BoschBrakeSystem());
    EXPECT_EQ(reg.brakeSystemCount(), 2);
}

TEST(PartRegistryTest, SteeringSystemCount_AfterRegister) {
    PartRegistry reg;
    reg.registerSteeringSystem(new BoschSteeringSystem());
    reg.registerSteeringSystem(new MobisSteeringSystem());
    EXPECT_EQ(reg.steeringSystemCount(), 2);
}

// 번호 → 구현체 반환

TEST(PartRegistryTest, GetCarType_ReturnsCorrectPart) {
    PartRegistry reg;
    reg.registerCarType(new Sedan());
    reg.registerCarType(new SUV());
    EXPECT_EQ(reg.getCarType(1)->getName(), "Sedan");
    EXPECT_EQ(reg.getCarType(2)->getName(), "SUV");
}

TEST(PartRegistryTest, GetEngine_ReturnsCorrectPart) {
    PartRegistry reg;
    reg.registerEngine(new GmEngine());
    reg.registerEngine(new BrokenEngine());
    EXPECT_EQ(reg.getEngine(1)->getName(), "GM");
    EXPECT_EQ(reg.getEngine(2)->getName(), "Broken");
}

// 범위 초과 시 nullptr

TEST(PartRegistryTest, GetCarType_ReturnsNullptr_WhenOutOfRange) {
    PartRegistry reg;
    reg.registerCarType(new Sedan());
    EXPECT_EQ(reg.getCarType(0), nullptr);
    EXPECT_EQ(reg.getCarType(2), nullptr);
}

TEST(PartRegistryTest, GetEngine_ReturnsNullptr_WhenOutOfRange) {
    PartRegistry reg;
    reg.registerEngine(new GmEngine());
    EXPECT_EQ(reg.getEngine(0), nullptr);
    EXPECT_EQ(reg.getEngine(2), nullptr);
}

TEST(PartRegistryTest, GetBrakeSystem_ReturnsNullptr_WhenOutOfRange) {
    PartRegistry reg;
    reg.registerBrakeSystem(new MandoBrakeSystem());
    EXPECT_EQ(reg.getBrakeSystem(0), nullptr);
    EXPECT_EQ(reg.getBrakeSystem(2), nullptr);
}

TEST(PartRegistryTest, GetSteeringSystem_ReturnsNullptr_WhenOutOfRange) {
    PartRegistry reg;
    reg.registerSteeringSystem(new BoschSteeringSystem());
    EXPECT_EQ(reg.getSteeringSystem(0), nullptr);
    EXPECT_EQ(reg.getSteeringSystem(2), nullptr);
}

#endif
