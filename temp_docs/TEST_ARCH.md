# 테스트 가능한 구조로의 전환 가이드

## 현재 코드의 문제점

### 1. 전역 상태 (`int stack[10]`)
`stack[]` 배열이 전역으로 선언되어 있어 모든 함수가 암묵적으로 공유합니다.
- 테스트 간 상태가 남아 격리가 불가능
- 테스트 순서에 따라 결과가 달라질 수 있음

```cpp
// 현재: 어떤 값이 들어있는지 테스트가 제어할 수 없음
int stack[10];

void selectCarType(int answer) {
    stack[CarType_Q] = answer;  // 전역 변수 직접 변경
    printf("...");              // 로직 + 출력 혼재
}
```

### 2. 로직과 I/O 혼재
`selectCarType()`, `runProducedCar()`, `testProducedCar()` 등 모든 함수가 상태 변경과 `printf()` 출력을 동시에 수행합니다.
- 로직을 검증하려면 반드시 출력이 발생함
- 출력 내용을 테스트에서 캡처하기 어려움

### 3. `isValidCheck()`가 전역 상태에 의존
```cpp
int isValidCheck() {
    if (stack[CarType_Q] == SEDAN && ...) { ... }  // 전역 stack 직접 참조
}
```
- 특정 조합을 검증하려면 전역 `stack[]`을 수동으로 세팅해야 함
- 순수 함수(Pure Function)가 아니므로 단위 테스트 작성이 번거로움

### 4. `delay()`가 로직 흐름에 내포
- 테스트 실행 속도를 크게 저하시킴

---

## 목표 구조

```
car_assemble/
├── Car.h / Car.cpp             ← 차량 설정 상태 보관
├── CarValidator.h / .cpp       ← 조합 유효성 검증 (순수 함수)
├── CarAssembler.h / .cpp       ← 조립 절차 (상태 변경 로직)
├── CarUI.h / .cpp              ← printf 등 출력 전담
├── main.cpp                    ← 진입점 (UI + Assembler 연결)
└── test/
    ├── CarValidatorTest.cpp
    └── CarAssemblerTest.cpp
```

---

## 변경 방법

### Step 1. 전역 상태를 구조체로 캡슐화

```cpp
// Car.h
struct CarConfig {
    int carType;
    int engine;
    int brakeSystem;
    int steeringSystem;
};
```

`stack[]` 대신 `CarConfig` 인스턴스를 함수 인자로 전달하거나 클래스 멤버로 보유합니다.

---

### Step 2. `isValidCheck()`를 순수 함수로 분리

```cpp
// CarValidator.h
bool isValidCombination(const CarConfig& config);
```

```cpp
// CarValidator.cpp
bool isValidCombination(const CarConfig& config) {
    if (config.carType == SEDAN && config.brakeSystem == CONTINENTAL) return false;
    if (config.carType == SUV   && config.engine == TOYOTA)           return false;
    if (config.carType == TRUCK && config.engine == WIA)              return false;
    if (config.carType == TRUCK && config.brakeSystem == MANDO)       return false;
    if (config.brakeSystem == BOSCH_B && config.steeringSystem != BOSCH_S) return false;
    return true;
}
```

전역 상태를 참조하지 않으므로 테스트에서 원하는 값을 직접 주입할 수 있습니다.

```cpp
// CarValidatorTest.cpp
TEST(CarValidatorTest, Sedan_Continental_Invalid) {
    CarConfig config = { SEDAN, GM, CONTINENTAL, BOSCH_S };
    EXPECT_FALSE(isValidCombination(config));
}

TEST(CarValidatorTest, Sedan_Mando_Valid) {
    CarConfig config = { SEDAN, GM, MANDO, BOSCH_S };
    EXPECT_TRUE(isValidCombination(config));
}
```

---

### Step 3. 조립 함수에서 출력 제거

```cpp
// CarAssembler.h
class CarAssembler {
public:
    void selectCarType(int answer);
    void selectEngine(int answer);
    void selectBrakeSystem(int answer);
    void selectSteeringSystem(int answer);
    const CarConfig& getConfig() const;
private:
    CarConfig config_ = {};
};
```

```cpp
// CarAssembler.cpp
void CarAssembler::selectCarType(int answer) {
    config_.carType = answer;   // 상태 변경만 수행, 출력 없음
}
```

조립 결과를 테스트에서 직접 확인할 수 있습니다.

```cpp
// CarAssemblerTest.cpp
TEST(CarAssemblerTest, SelectCarType_Sedan) {
    CarAssembler assembler;
    assembler.selectCarType(SEDAN);
    EXPECT_EQ(assembler.getConfig().carType, SEDAN);
}
```

---

### Step 4. 출력은 CarUI에서 전담

```cpp
// CarUI.h
class CarUI {
public:
    void printCarTypeMenu();
    void printEngineMenu();
    void printBrakeMenu();
    void printSteeringMenu();
    void printResult(const CarConfig& config, bool isValid);
    void printTestResult(const CarConfig& config, bool isValid);
};
```

로직 함수에서 `printf()`를 완전히 제거하여 로직 테스트 시 출력이 발생하지 않습니다.

---

### Step 5. main()은 연결만 담당

```cpp
// main.cpp
int main() {
    CarAssembler assembler;
    CarUI ui;

    // UI 입력을 받아 assembler에 전달하고, 결과를 ui로 출력
    ui.printCarTypeMenu();
    int answer = ui.getInput();
    assembler.selectCarType(answer);
    // ...
}
```

---

## 변경 전/후 테스트 가능성 비교

| 항목 | 현재 | 변경 후 |
|------|------|---------|
| `isValidCheck()` 단위 테스트 | 전역 `stack[]` 직접 세팅 필요 | `CarConfig` 값만 넘기면 됨 |
| 조합 격리 | 테스트마다 `stack[]` 초기화 필요 | 테스트마다 독립된 인스턴스 사용 |
| 출력 없이 로직 검증 | 불가 | 가능 |
| delay() 제거 | 테스트 속도 저하 | 로직 함수에서 분리되어 영향 없음 |

---

## 우선순위

1. **`CarConfig` 구조체 도입** (전역 상태 제거) — 가장 영향 범위가 큼
2. **`isValidCombination()` 순수 함수화** — 즉시 테스트 작성 가능
3. **select* 함수에서 printf 제거** — 로직 테스트 격리
4. **CarUI 분리** — 출력 테스트가 필요한 경우 Mock 적용 가능
