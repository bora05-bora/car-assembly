#ifdef _DEBUG

#include "gmock/gmock.h"

#include "../Car.h"
#include "../car_types/Sedan.h"
#include "../car_types/SUV.h"
#include "../car_types/Truck.h"
#include "../engines/GmEngine.h"
#include "../engines/ToyotaEngine.h"
#include "../engines/WiaEngine.h"
#include "../brake_systems/MandoBrakeSystem.h"
#include "../brake_systems/ContinentalBrakeSystem.h"
#include "../brake_systems/BoschBrakeSystem.h"
#include "../steering_systems/BoschSteeringSystem.h"
#include "../steering_systems/MobisSteeringSystem.h"

// isFullyAssembled

TEST(CarTest, IsFullyAssembled_ReturnsFalse_WhenNoPartsSet) {
    Car car;
    EXPECT_FALSE(car.isFullyAssembled());
}

TEST(CarTest, IsFullyAssembled_ReturnsFalse_WhenTypeOnly) {
    Car car;
    car.setType(new Sedan());
    EXPECT_FALSE(car.isFullyAssembled());
}

TEST(CarTest, IsFullyAssembled_ReturnsFalse_WhenEngineMissing) {
    Car car;
    car.setType(new Sedan());
    car.setBrakeSystem(new MandoBrakeSystem());
    car.setSteeringSystem(new BoschSteeringSystem());
    EXPECT_FALSE(car.isFullyAssembled());
}

TEST(CarTest, IsFullyAssembled_ReturnsTrue_WhenAllPartsSet) {
    Car car;
    car.setType(new Sedan());
    car.setEngine(new GmEngine());
    car.setBrakeSystem(new MandoBrakeSystem());
    car.setSteeringSystem(new BoschSteeringSystem());
    EXPECT_TRUE(car.isFullyAssembled());
}

// getter

TEST(CarTest, GetType_ReturnsSetPart) {
    Car car;
    ICarType* sedan = new Sedan();
    car.setType(sedan);
    EXPECT_EQ(car.getType(), sedan);
}

TEST(CarTest, GetEngine_ReturnsSetPart) {
    Car car;
    IEngine* engine = new GmEngine();
    car.setEngine(engine);
    EXPECT_EQ(car.getEngine(), engine);
}

TEST(CarTest, GetBrakeSystem_ReturnsSetPart) {
    Car car;
    IBrakeSystem* brake = new MandoBrakeSystem();
    car.setBrakeSystem(brake);
    EXPECT_EQ(car.getBrakeSystem(), brake);
}

TEST(CarTest, GetSteeringSystem_ReturnsSetPart) {
    Car car;
    ISteeringSystem* steering = new BoschSteeringSystem();
    car.setSteeringSystem(steering);
    EXPECT_EQ(car.getSteeringSystem(), steering);
}

#endif
