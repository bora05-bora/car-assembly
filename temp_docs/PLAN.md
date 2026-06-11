# Refactoring Plan — car_assemble (OCP 적용)

## 설계 목표

> **OCP (Open/Closed Principle)**: 새로운 차량 타입·부품·조합 규칙을 추가할 때  
> **기존 코드를 수정하지 않고(Closed)** 새 클래스만 추가(Open)하는 구조.

현재 코드의 문제 (PDF p.24):
- 절차지향 코드 → 부품/타입 추가 시 `if`/`switch` 전체 수정 필요
- 안전하지 않은 문법 사용
- 전역 `stack[]`에 의존 → 테스트 격리 불가
- 로직과 `printf()` 출력 혼재 → 로직만 단독 검증 불가
- 확장성 없음, 유닛 테스트 없음

리팩토링은 **Method Level → Interface 정의 → 구현체 분리 → 조합 검증 → 통합** 순서로 진행합니다.

> **테스트 작성 시점**: Phase 3(구현체 분리) 완료 후부터 각 Phase 종료 시점마다 해당 클래스의 단위 테스트를 함께 작성합니다. Phase 8은 통합 및 누락 케이스 보완 단계입니다.  
> Phase 1~2는 전역 상태·추상 인터페이스만 다루므로 단위 테스트 대상이 없습니다.

---

## Phase 1 — Method Level 정리 (기존 코드 안에서)

인터페이스 설계 전에 현재 코드의 명백한 결함부터 제거합니다.

### 1-1. 안전하지 않은 문법 수정

| 위치 | 문제 | 수정 |
|------|------|------|
| `strtok_s` 두 번째 호출에 `buf` 재전달 | 이미 분리된 토큰 덮어씀 | `buf[strcspn(buf, "\r\n")] = '\0'` 한 줄로 대체 |
| `delay()` — `volatile` busy-wait 3중 루프 | CPU 100% 점유, OS 시간 보장 없음, **테스트 속도 저하** | `Sleep(ms)` (Windows API)로 교체 |
| `isValidCheck()` 끝 중복 `return true` | 도달 불가 dead code | 제거 |
| `isValidCheck()` 반환 타입 `int` | 의미상 `bool` | `bool`로 변경 |

### 1-2. 네이밍 컨벤션 통일

| 현재 | 변경 | 이유 |
|------|------|------|
| `selectbrakeSystem` | `selectBrakeSystem` | camelCase 일관성 |
| `isValidCheck` | `isValidCombination` | "Check" 중복 |
| 전역 `stack[]` | `g_selection[]` | `stack` 자료구조 명칭 충돌 |

### 1-3. 반복 `if` 체인 → `switch-case`

- `selectCarType`, `selectEngine`, `selectBrakeSystem`, `selectSteeringSystem` 내부
- `runProducedCar()` 내 부품 출력 블록
- 입력 유효 범위 검사 → `isInRange(int val, int lo, int hi)` 헬퍼 추출

---

## Phase 2 — Interface 정의 (순수 추상 클래스)

OCP의 핵심 축. **이 인터페이스만 참조하면 구체 클래스가 몇 개든 기존 코드를 건드리지 않아도 됩니다.**

### 2-1. `ICarType`

```cpp
// ICarType.h
class ICarType {
public:
    virtual ~ICarType() = default;
    virtual std::string getName() const = 0;  // "Sedan" / "SUV" / "Truck"
};
```

### 2-2. `IEngine`

```cpp
// IEngine.h
class IEngine {
public:
    virtual ~IEngine() = default;
    virtual std::string getName() const = 0;   // "GM" / "TOYOTA" / "WIA"
    virtual bool isBroken() const = 0;         // 고장 엔진 판별
};
```

### 2-3. `IBrakeSystem`

```cpp
// IBrakeSystem.h
class IBrakeSystem {
public:
    virtual ~IBrakeSystem() = default;
    virtual std::string getName() const = 0;   // "Mando" / "Continental" / "Bosch"
};
```

### 2-4. `ISteeringSystem`

```cpp
// ISteeringSystem.h
class ISteeringSystem {
public:
    virtual ~ISteeringSystem() = default;
    virtual std::string getName() const = 0;   // "Bosch" / "Mobis"
};
```

### 2-5. `ICombinationRule`

조합 제한조건도 인터페이스화합니다. 새 규칙 = 새 클래스 추가만으로 끝납니다.

```cpp
// ICombinationRule.h
class ICombinationRule {
public:
    virtual ~ICombinationRule() = default;
    virtual bool isViolated(const Car& car) const = 0;
    virtual std::string getFailReason() const = 0;
};
```

---

## Phase 3 — 구현체 분리

각 인터페이스의 구체 클래스를 개별 파일로 작성합니다.

### 3-1. CarType 구현체

```
car_types/
├── Sedan.h/.cpp    — ICarType, getName() = "Sedan"
├── SUV.h/.cpp      — ICarType, getName() = "SUV"
└── Truck.h/.cpp    — ICarType, getName() = "Truck"
```

> 신규 타입 추가: `Hatchback.h/.cpp` 파일 추가 → 기존 파일 무수정

### 3-2. Engine 구현체

```
engines/
├── GmEngine.h/.cpp      — IEngine, isBroken() = false
├── ToyotaEngine.h/.cpp  — IEngine, isBroken() = false
├── WiaEngine.h/.cpp     — IEngine, isBroken() = false
└── BrokenEngine.h/.cpp  — IEngine, isBroken() = true
```

### 3-3. BrakeSystem 구현체

```
brake_systems/
├── MandoBrakeSystem.h/.cpp
├── ContinentalBrakeSystem.h/.cpp
└── BoschBrakeSystem.h/.cpp
```

### 3-4. SteeringSystem 구현체

```
steering_systems/
├── BoschSteeringSystem.h/.cpp
└── MobisSteeringSystem.h/.cpp
```

### 3-5. CombinationRule 구현체

PDF p.23의 제한조건 5가지를 각각 독립 클래스로 분리합니다.

```
rules/
├── BoschBrakeRequiresBoschSteeringRule.h/.cpp
│     — Bosch 제동장치 → Bosch 조향장치 강제
├── SedanCannotUseContinentalRule.h/.cpp
│     — Sedan + Continental 조합 금지
├── SuvCannotUseToyotaRule.h/.cpp
│     — SUV + Toyota 조합 금지
├── TruckCannotUseWiaRule.h/.cpp
│     — Truck + WIA 조합 금지
└── TruckCannotUseMandoRule.h/.cpp
      — Truck + Mando 조합 금지
```

> 신규 규칙 추가: 새 Rule 클래스 작성 후 등록만 하면 됨 → `CombinationValidator` 수정 불필요

**Phase 3 완료 후 작성할 테스트:**
```cpp
// 구현체 getName() 검증
TEST(SedanTest, GetName) {
    Sedan sedan;
    EXPECT_EQ(sedan.getName(), "Sedan");
}

// BrokenEngine 판별
TEST(BrokenEngineTest, IsBroken_ReturnsTrue) {
    BrokenEngine engine;
    EXPECT_TRUE(engine.isBroken());
}

TEST(GmEngineTest, IsBroken_ReturnsFalse) {
    GmEngine engine;
    EXPECT_FALSE(engine.isBroken());
}
```

전역 상태도, `printf()`도 없는 독립 클래스이므로 이 시점부터 단위 테스트가 가능합니다.

---

## Phase 4 — `Car` 클래스 (전역 `stack[]` 제거)

조립 상태를 인터페이스 포인터로 보관합니다.

```cpp
// Car.h
class Car {
    ICarType*        type_         = nullptr;
    IEngine*         engine_       = nullptr;
    IBrakeSystem*    brakeSystem_  = nullptr;
    ISteeringSystem* steering_     = nullptr;
public:
    void setType(ICarType*);
    void setEngine(IEngine*);
    void setBrakeSystem(IBrakeSystem*);
    void setSteeringSystem(ISteeringSystem*);

    const ICarType*        getType()          const;
    const IEngine*         getEngine()        const;
    const IBrakeSystem*    getBrakeSystem()   const;
    const ISteeringSystem* getSteeringSystem()const;

    bool isFullyAssembled() const;  // 4개 부품이 모두 세팅됐는지
};
```

**Phase 4 완료 후 작성할 테스트:**
```cpp
TEST(CarTest, IsFullyAssembled_ReturnsFalse_WhenPartMissing) {
    Car car;
    car.setType(new Sedan());
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
```

---

## Phase 5 — `CombinationValidator` (Rule 목록 기반)

`ICombinationRule` 목록을 순회하며 검증합니다.  
새 규칙 추가 시 **이 클래스를 수정하지 않습니다**.

```cpp
// CombinationValidator.h
class CombinationValidator {
    std::vector<ICombinationRule*> rules_;
public:
    void addRule(ICombinationRule* rule);

    bool        isValid(const Car& car) const;
    std::string getFailReason(const Car& car) const;
};
```

**Phase 5 완료 후 작성할 테스트 (핵심 — 현재 `isValidCheck()`의 전역 상태 의존 해소):**
```cpp
// 각 Rule 클래스 단독 검증
TEST(SedanCannotUseContinentalRuleTest, Violated) {
    Car car;
    car.setType(new Sedan());
    car.setBrakeSystem(new ContinentalBrakeSystem());
    EXPECT_TRUE(SedanCannotUseContinentalRule().isViolated(car));
}

// Mock으로 Validator 로직만 격리 검증
TEST(CombinationValidatorTest, CallsAllRules) {
    MockCombinationRule mockRule;
    EXPECT_CALL(mockRule, isViolated(_)).WillOnce(Return(false));

    CombinationValidator validator;
    validator.addRule(&mockRule);

    Car car; /* 부품 세팅 */
    validator.isValid(car);
}
```

---

## Phase 6 — `PartRegistry` (선택지 등록·조회)

메뉴 번호 → 구현체 매핑을 레지스트리에서 관리합니다.  
새 부품 추가 = `register*()` 호출 한 줄 → 메뉴에 자동 반영됩니다.

```cpp
// PartRegistry.h
class PartRegistry {
    std::vector<ICarType*>        carTypes_;
    std::vector<IEngine*>         engines_;
    std::vector<IBrakeSystem*>    brakeSystems_;
    std::vector<ISteeringSystem*> steeringSystems_;
public:
    void registerCarType(ICarType*);
    void registerEngine(IEngine*);
    void registerBrakeSystem(IBrakeSystem*);
    void registerSteeringSystem(ISteeringSystem*);

    // 메뉴 출력용
    void printCarTypes()       const;
    void printEngines()        const;
    void printBrakeSystems()   const;
    void printSteeringSystems()const;

    // 번호 → 구현체 반환 (범위 초과 시 nullptr)
    ICarType*        getCarType(int idx)        const;
    IEngine*         getEngine(int idx)         const;
    IBrakeSystem*    getBrakeSystem(int idx)    const;
    ISteeringSystem* getSteeringSystem(int idx) const;

    // 유효 범위 반환 (입력 검증용)
    int carTypeCount()        const;
    int engineCount()         const;
    int brakeSystemCount()    const;
    int steeringSystemCount() const;
};
```

---

## Phase 7 — `CarUI` 분리 + `CarAssembler` 통합

### 7-1. `CarUI` — 출력 전담 (로직과 완전 분리)

현재 `selectCarType()` 등의 함수가 상태 변경과 `printf()`를 동시에 수행하는 문제를 해결합니다.  
**`CarAssembler`에서 출력이 사라지면 조립 로직만 단독으로 테스트할 수 있습니다.**

```cpp
// CarUI.h
class CarUI {
public:
    void printCarTypeMenu(const PartRegistry& registry) const;
    void printEngineMenu(const PartRegistry& registry) const;
    void printBrakeMenu(const PartRegistry& registry) const;
    void printSteeringMenu(const PartRegistry& registry) const;
    void printRunResult(const Car& car) const;
    void printTestResult(bool isValid, const std::string& reason) const;
    int  getInput() const;
};
```

### 7-2. `CarAssembler` — 조립 흐름 통합

`PartRegistry`와 `CombinationValidator`를 주입받아 사용합니다.  
출력은 `CarUI`에 위임하므로 이 클래스에는 `printf()` 호출이 없습니다.

```cpp
// CarAssembler.h
class CarAssembler {
    Car                   car_;
    PartRegistry&         registry_;
    CombinationValidator& validator_;
    CarUI                 ui_;
    int                   step_;
public:
    CarAssembler(PartRegistry& registry, CombinationValidator& validator);

    void run();
private:
    bool handleInput(int answer);
    void runCar()  const;
    void testCar() const;
};
```

`main()`은 등록 코드와 실행 한 줄만 남습니다:

```cpp
int main() {
    PartRegistry registry;
    registry.registerCarType(new Sedan());
    registry.registerCarType(new SUV());
    registry.registerCarType(new Truck());
    registry.registerEngine(new GmEngine());
    // ... 나머지 등록

    CombinationValidator validator;
    validator.addRule(new BoschBrakeRequiresBoschSteeringRule());
    // ... 나머지 규칙 등록

    CarAssembler assembler(registry, validator);
    assembler.run();
}
```

---

## Phase 8 — 단위 테스트 보완 및 통합

Google Mock 1.11.0 (패키지에 포함됨)을 사용합니다.  
`#ifdef _DEBUG` 블록을 복원하여 Debug 빌드에서 실행합니다.  
Phase 4~7에서 작성한 테스트를 통합하고 누락된 케이스를 보완합니다.

| 테스트 대상 | 케이스 | 작성 시점 |
|-------------|--------|-----------|
| `Car::isFullyAssembled` | 부품 누락 → false, 완성 → true | Phase 4 |
| 각 Rule 클래스 `isViolated` | 위반 조합 → true, 정상 조합 → false | Phase 5 |
| `CombinationValidator::isValid` | 5가지 위반 + 다수 정상 케이스 | Phase 5 |
| `CombinationValidator::getFailReason` | 위반 시 올바른 사유 문자열 반환 | Phase 5 |
| 각 구현체 `getName()` | 올바른 이름 문자열 반환 | Phase 3 |
| `BrokenEngine::isBroken` | true 반환 확인 | Phase 3 |
| Mock 활용 | `ICombinationRule` Mock으로 Validator 로직 독립 검증 | Phase 5 |
| `CarAssembler::handleInput` | 잘못된 범위 입력 → 상태 변경 없음 | Phase 7 이후 |
| 경계값 | 선택지 최솟값·최댓값·범위 초과 | Phase 8 |

---

## 최종 파일 구조

```
car_assemble/
├── interfaces/
│   ├── ICarType.h
│   ├── IEngine.h
│   ├── IBrakeSystem.h
│   ├── ISteeringSystem.h
│   └── ICombinationRule.h
├── car_types/
│   ├── Sedan.h/.cpp
│   ├── SUV.h/.cpp
│   └── Truck.h/.cpp
├── engines/
│   ├── GmEngine.h/.cpp
│   ├── ToyotaEngine.h/.cpp
│   ├── WiaEngine.h/.cpp
│   └── BrokenEngine.h/.cpp
├── brake_systems/
│   ├── MandoBrakeSystem.h/.cpp
│   ├── ContinentalBrakeSystem.h/.cpp
│   └── BoschBrakeSystem.h/.cpp
├── steering_systems/
│   ├── BoschSteeringSystem.h/.cpp
│   └── MobisSteeringSystem.h/.cpp
├── rules/
│   ├── BoschBrakeRequiresBoschSteeringRule.h/.cpp
│   ├── SedanCannotUseContinentalRule.h/.cpp
│   ├── SuvCannotUseToyotaRule.h/.cpp
│   ├── TruckCannotUseWiaRule.h/.cpp
│   └── TruckCannotUseMandoRule.h/.cpp
├── Car.h/.cpp
├── CombinationValidator.h/.cpp
├── PartRegistry.h/.cpp
├── CarUI.h/.cpp                  ← 출력 전담 (printf 집중)
├── CarAssembler.h/.cpp           ← 조립 흐름 (printf 없음)
└── main.cpp
```

---

## 진행 순서 요약

```
Phase 1   Method Level 정리
          (unsafe 문법, 네이밍, switch-case, delay → Sleep)
    ↓
Phase 2   Interface 정의
          (ICarType / IEngine / IBrakeSystem / ISteeringSystem / ICombinationRule)
    ↓
Phase 3   구현체 분리
          (CarType 3종 / Engine 4종 / BrakeSystem 3종 / SteeringSystem 2종 / Rule 5종)
          + 테스트 작성 시작 (getName, isBroken 등 독립 클래스 검증)
    ↓
Phase 4   Car 클래스 (전역 stack[] 제거) + Car 테스트
    ↓
Phase 5   CombinationValidator (Rule 목록 순회) + Rule/Validator 테스트
    ↓
Phase 6   PartRegistry (메뉴·선택지 등록 관리)
    ↓
Phase 7   CarUI 분리 (printf 집중) + CarAssembler 통합 (printf 제거)
    ↓
Phase 8   테스트 보완 (경계값, 누락 케이스, Mock 활용 통합 검증)
```

**각 Phase 완료 체크포인트:**
- [ ] Debug/Release 빌드 모두 성공
- [ ] 기존 동작(메뉴 흐름, 조합 검증 결과) 변경 없음
- [ ] 해당 Phase에서 추가한 클래스의 단위 테스트 통과

> OCP 확인 기준: 새 차량 타입·부품·조합 규칙 추가 시 기존 `.cpp` 파일을 열어 수정하는 일이 없어야 합니다.  
> 테스트 확인 기준: 전역 상태 없이, 출력 없이 조합 유효성 검증 로직만 단독으로 실행 가능해야 합니다.
