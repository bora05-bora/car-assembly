#ifdef _DEBUG

#include "gmock/gmock.h"

#include "../Car.h"
#include "../CombinationValidator.h"
#include "../interfaces/ICombinationRule.h"
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
#include "../rules/SedanCannotUseContinentalRule.h"
#include "../rules/SuvCannotUseToyotaRule.h"
#include "../rules/TruckCannotUseWiaRule.h"
#include "../rules/TruckCannotUseMandoRule.h"
#include "../rules/BoschBrakeRequiresBoschSteeringRule.h"

using ::testing::_;
using ::testing::Return;

// --- Mock ---

class MockCombinationRule : public ICombinationRule {
public:
    MOCK_METHOD(bool,        isViolated,    (const Car& car), (const, override));
    MOCK_METHOD(std::string, getFailReason, (),               (const, override));
};

// --- Rule 단독 검증 ---

TEST(RuleTest, Sedan_Continental_Violated) {
    Car car;
    car.setType(new Sedan());
    car.setBrakeSystem(new ContinentalBrakeSystem());
    EXPECT_TRUE(SedanCannotUseContinentalRule().isViolated(car));
}

TEST(RuleTest, Sedan_Mando_NotViolated) {
    Car car;
    car.setType(new Sedan());
    car.setBrakeSystem(new MandoBrakeSystem());
    EXPECT_FALSE(SedanCannotUseContinentalRule().isViolated(car));
}

TEST(RuleTest, SUV_Toyota_Violated) {
    Car car;
    car.setType(new SUV());
    car.setEngine(new ToyotaEngine());
    EXPECT_TRUE(SuvCannotUseToyotaRule().isViolated(car));
}

TEST(RuleTest, SUV_Gm_NotViolated) {
    Car car;
    car.setType(new SUV());
    car.setEngine(new GmEngine());
    EXPECT_FALSE(SuvCannotUseToyotaRule().isViolated(car));
}

TEST(RuleTest, Truck_Wia_Violated) {
    Car car;
    car.setType(new Truck());
    car.setEngine(new WiaEngine());
    EXPECT_TRUE(TruckCannotUseWiaRule().isViolated(car));
}

TEST(RuleTest, Truck_Gm_NotViolated) {
    Car car;
    car.setType(new Truck());
    car.setEngine(new GmEngine());
    EXPECT_FALSE(TruckCannotUseWiaRule().isViolated(car));
}

TEST(RuleTest, Truck_Mando_Violated) {
    Car car;
    car.setType(new Truck());
    car.setBrakeSystem(new MandoBrakeSystem());
    EXPECT_TRUE(TruckCannotUseMandoRule().isViolated(car));
}

TEST(RuleTest, Truck_Bosch_NotViolated) {
    Car car;
    car.setType(new Truck());
    car.setBrakeSystem(new BoschBrakeSystem());
    EXPECT_FALSE(TruckCannotUseMandoRule().isViolated(car));
}

TEST(RuleTest, BoschBrake_MobisSteering_Violated) {
    Car car;
    car.setBrakeSystem(new BoschBrakeSystem());
    car.setSteeringSystem(new MobisSteeringSystem());
    EXPECT_TRUE(BoschBrakeRequiresBoschSteeringRule().isViolated(car));
}

TEST(RuleTest, BoschBrake_BoschSteering_NotViolated) {
    Car car;
    car.setBrakeSystem(new BoschBrakeSystem());
    car.setSteeringSystem(new BoschSteeringSystem());
    EXPECT_FALSE(BoschBrakeRequiresBoschSteeringRule().isViolated(car));
}

// --- CombinationValidator: Mock으로 Validator 로직 격리 검증 ---

TEST(CombinationValidatorTest, IsValid_ReturnsTrue_WhenNoRulesViolated) {
    MockCombinationRule mockRule;
    EXPECT_CALL(mockRule, isViolated(_)).WillOnce(Return(false));

    CombinationValidator validator;
    validator.addRule(&mockRule);

    Car car;
    EXPECT_TRUE(validator.isValid(car));
}

TEST(CombinationValidatorTest, IsValid_ReturnsFalse_WhenRuleViolated) {
    MockCombinationRule mockRule;
    EXPECT_CALL(mockRule, isViolated(_)).WillOnce(Return(true));

    CombinationValidator validator;
    validator.addRule(&mockRule);

    Car car;
    EXPECT_FALSE(validator.isValid(car));
}

TEST(CombinationValidatorTest, GetFailReason_ReturnsReason_WhenViolated) {
    MockCombinationRule mockRule;
    EXPECT_CALL(mockRule, isViolated(_)).WillOnce(Return(true));
    EXPECT_CALL(mockRule, getFailReason()).WillOnce(Return("테스트 위반 사유"));

    CombinationValidator validator;
    validator.addRule(&mockRule);

    Car car;
    EXPECT_EQ(validator.getFailReason(car), "테스트 위반 사유");
}

TEST(CombinationValidatorTest, GetFailReason_ReturnsEmpty_WhenNotViolated) {
    MockCombinationRule mockRule;
    EXPECT_CALL(mockRule, isViolated(_)).WillOnce(Return(false));

    CombinationValidator validator;
    validator.addRule(&mockRule);

    Car car;
    EXPECT_EQ(validator.getFailReason(car), "");
}

TEST(CombinationValidatorTest, StopsAtFirstViolatedRule) {
    MockCombinationRule rule1, rule2;
    EXPECT_CALL(rule1, isViolated(_)).WillOnce(Return(true));
    EXPECT_CALL(rule2, isViolated(_)).Times(0);

    CombinationValidator validator;
    validator.addRule(&rule1);
    validator.addRule(&rule2);

    Car car;
    EXPECT_FALSE(validator.isValid(car));
}

#endif
