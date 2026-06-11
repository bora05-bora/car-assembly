# car_assemble

콘솔 기반 자동차 조립 시뮬레이터입니다. 사용자가 단계별로 부품을 선택하여 자동차를 조립하고, 조합의 유효성을 검증합니다.

## 프로젝트 구조

```
car_assemble/
├── car_assemble.slnx              # Visual Studio 솔루션 파일
├── car_assemble/
│   ├── main.cpp                   # 메인 소스 코드
│   ├── car_assemble.vcxproj       # Visual Studio 프로젝트 파일
│   └── packages.config            # NuGet 패키지 설정
└── packages/
    └── gmock.1.11.0/              # Google Mock 테스트 프레임워크
```

## 빌드 환경

- **언어**: C (C99 스타일, MSVC 컴파일러)
- **IDE**: Visual Studio (`.slnx` 솔루션)
- **테스트**: Google Mock 1.11.0

## 실행 방법

Visual Studio에서 솔루션을 열고 빌드 후 실행하거나, x64 빌드 디렉토리의 실행 파일을 직접 실행합니다.

```
exit  # 프로그램 종료
```

## 조립 단계

프로그램은 다음 순서로 부품 선택을 안내합니다.

| 단계 | 선택 항목 | 옵션 |
|------|-----------|------|
| 1 | 차량 타입 | Sedan, SUV, Truck |
| 2 | 엔진 | GM, TOYOTA, WIA, 고장난 엔진 |
| 3 | 제동장치 | MANDO, CONTINENTAL, BOSCH |
| 4 | 조향장치 | BOSCH, MOBIS |
| 5 | 동작 | RUN 또는 Test |

각 단계에서 `0`을 입력하면 이전 단계로 돌아갑니다 (차량 타입 선택 제외).

## 부품 조합 제약 규칙

아래 조합은 유효하지 않으며, Test 시 FAIL 판정을 받습니다.

| 차량 타입 | 사용 불가 부품 | 이유 |
|-----------|----------------|------|
| Sedan | CONTINENTAL 제동장치 | 호환 불가 |
| SUV | TOYOTA 엔진 | 호환 불가 |
| Truck | WIA 엔진 | 호환 불가 |
| Truck | MANDO 제동장치 | 호환 불가 |
| (공통) BOSCH 제동장치 | BOSCH 이외 조향장치 | BOSCH 제동장치는 BOSCH 조향장치 전용 |

## RUN vs Test

- **RUN**: 현재 조합으로 자동차를 실제 구동합니다. 유효하지 않은 조합이면 "자동차가 동작되지 않습니다"를 출력합니다.
- **Test**: 부품 조합의 유효성을 검증하고 PASS / FAIL 결과와 실패 원인을 출력합니다.
