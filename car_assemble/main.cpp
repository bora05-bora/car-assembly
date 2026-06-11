#ifdef _DEBUG

#include "gmock/gmock.h"

int main()
{
    testing::InitGoogleMock();
    return RUN_ALL_TESTS();
}

#else

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>

#define CLEAR_SCREEN "\033[H\033[2J"

int g_selection[10];

void selectCarType(int answer);
void selectEngine(int answer);
void selectBrakeSystem(int answer);
void selectSteeringSystem(int answer);
void runProducedCar();
void testProducedCar();
bool isInRange(int val, int lo, int hi);
bool isValidCombination();

enum QuestionType
{
    CarType_Q,
    Engine_Q,
    brakeSystem_Q,
    SteeringSystem_Q,
    Run_Test,
};

enum CarType
{
    SEDAN = 1,
    SUV,
    TRUCK
};

enum Engine
{
    GM = 1,
    TOYOTA,
    WIA
};

enum brakeSystem
{
    MANDO = 1,
    CONTINENTAL,
    BOSCH_B
};

enum SteeringSystem
{
    BOSCH_S = 1,
    MOBIS
};

bool isInRange(int val, int lo, int hi)
{
    return val >= lo && val <= hi;
}

int main()
{
    char buf[100];
    int step = CarType_Q;

    while (1)
    {

        if (step == CarType_Q)
        {
            printf(CLEAR_SCREEN);

            printf("        ______________\n");
            printf("       /|            | \n");
            printf("  ____/_|_____________|____\n");
            printf(" |                      O  |\n");
            printf(" '-(@)----------------(@)--'\n");
            printf("===============================\n");
            printf("어떤 차량 타입을 선택할까요?\n");
            printf("1. Sedan\n");
            printf("2. SUV\n");
            printf("3. Truck\n");
        }
        else if (step == Engine_Q)
        {
            printf(CLEAR_SCREEN);
            printf("어떤 엔진을 탑재할까요?\n");
            printf("0. 뒤로가기\n");
            printf("1. GM\n");
            printf("2. TOYOTA\n");
            printf("3. WIA\n");
            printf("4. 고장난 엔진\n");
        }
        else if (step == brakeSystem_Q)
        {
            printf(CLEAR_SCREEN);
            printf("어떤 제동장치를 선택할까요?\n");
            printf("0. 뒤로가기\n");
            printf("1. MANDO\n");
            printf("2. CONTINENTAL\n");
            printf("3. BOSCH\n");
        }
        else if (step == SteeringSystem_Q)
        {
            printf(CLEAR_SCREEN);
            printf("어떤 조향장치를 선택할까요?\n");
            printf("0. 뒤로가기\n");
            printf("1. BOSCH\n");
            printf("2. MOBIS\n");
        }
        else if (step == Run_Test)
        {
            printf(CLEAR_SCREEN);
            printf("멋진 차량이 완성되었습니다.\n");
            printf("어떤 동작을 할까요?\n");
            printf("0. 처음 화면으로 돌아가기\n");
            printf("1. RUN\n");
            printf("2. Test\n");
        }
        printf("===============================\n");

        printf("INPUT > ");
        fgets(buf, sizeof(buf), stdin);

        // 엔터 개행문자 제거
        buf[strcspn(buf, "\r\n")] = '\0';

        if (!strcmp(buf, "exit"))
        {
            printf("바이바이\n");
            break;
        }

        // 숫자로 된 대답인지 확인
        char* checkNumber;
        int answer = strtol(buf, &checkNumber, 10);

        if (*checkNumber != '\0')
        {
            printf("ERROR :: 숫자만 입력 가능\n");
            Sleep(800);
            continue;
        }

        if (step == CarType_Q && !isInRange(answer, 1, 3))
        {
            printf("ERROR :: 차량 타입은 1 ~ 3 범위만 선택 가능\n");
            Sleep(800);
            continue;
        }

        if (step == Engine_Q && !isInRange(answer, 0, 4))
        {
            printf("ERROR :: 엔진은 1 ~ 4 범위만 선택 가능\n");
            Sleep(800);
            continue;
        }

        if (step == brakeSystem_Q && !isInRange(answer, 0, 3))
        {
            printf("ERROR :: 제동장치는 1 ~ 3 범위만 선택 가능\n");
            Sleep(800);
            continue;
        }

        if (step == SteeringSystem_Q && !isInRange(answer, 0, 2))
        {
            printf("ERROR :: 조향장치는 1 ~ 2 범위만 선택 가능\n");
            Sleep(800);
            continue;
        }

        if (step == Run_Test && !isInRange(answer, 0, 2))
        {
            printf("ERROR :: Run 또는 Test 중 하나를 선택 필요\n");
            Sleep(800);
            continue;
        }

        // 처음으로 돌아가기
        if (answer == 0 && step == Run_Test)
        {
            step = CarType_Q;
            continue;
        }

        // 이전으로 돌아가기
        if (answer == 0 && step >= 1)
        {
            step -= 1;
            continue;
        }

        if (step == CarType_Q)
        {
            selectCarType(answer);
            Sleep(800);
            step = Engine_Q;
        }
        else if (step == Engine_Q)
        {
            selectEngine(answer);
            Sleep(800);
            step = brakeSystem_Q;
        }
        else if (step == brakeSystem_Q)
        {
            selectBrakeSystem(answer);
            Sleep(800);
            step = SteeringSystem_Q;
        }
        else if (step == SteeringSystem_Q)
        {
            selectSteeringSystem(answer);
            Sleep(800);
            step = Run_Test;
        }
        else if (step == Run_Test && answer == 1)
        {
            runProducedCar();
            Sleep(2000);
        }
        else if (step == Run_Test && answer == 2)
        {
            printf("Test...\n");
            Sleep(1500);
            testProducedCar();
            Sleep(2000);
        }
    }
}

void selectCarType(int answer)
{
    g_selection[CarType_Q] = answer;
    switch (answer)
    {
    case SEDAN: printf("차량 타입으로 Sedan을 선택하셨습니다.\n"); break;
    case SUV:   printf("차량 타입으로 SUV을 선택하셨습니다.\n");   break;
    case TRUCK: printf("차량 타입으로 Truck을 선택하셨습니다.\n"); break;
    }
}

void selectEngine(int answer)
{
    g_selection[Engine_Q] = answer;
    switch (answer)
    {
    case GM:     printf("GM 엔진을 선택하셨습니다.\n");     break;
    case TOYOTA: printf("TOYOTA 엔진을 선택하셨습니다.\n"); break;
    case WIA:    printf("WIA 엔진을 선택하셨습니다.\n");    break;
    }
}

void selectBrakeSystem(int answer)
{
    g_selection[brakeSystem_Q] = answer;
    switch (answer)
    {
    case MANDO:       printf("MANDO 제동장치를 선택하셨습니다.\n");       break;
    case CONTINENTAL: printf("CONTINENTAL 제동장치를 선택하셨습니다.\n"); break;
    case BOSCH_B:     printf("BOSCH 제동장치를 선택하셨습니다.\n");       break;
    }
}

void selectSteeringSystem(int answer)
{
    g_selection[SteeringSystem_Q] = answer;
    switch (answer)
    {
    case BOSCH_S: printf("BOSCH 조향장치를 선택하셨습니다.\n"); break;
    case MOBIS:   printf("MOBIS 조향장치를 선택하셨습니다.\n"); break;
    }
}

bool isValidCombination()
{
    if (g_selection[CarType_Q] == SEDAN && g_selection[brakeSystem_Q] == CONTINENTAL) return false;
    if (g_selection[CarType_Q] == SUV   && g_selection[Engine_Q] == TOYOTA)           return false;
    if (g_selection[CarType_Q] == TRUCK && g_selection[Engine_Q] == WIA)              return false;
    if (g_selection[CarType_Q] == TRUCK && g_selection[brakeSystem_Q] == MANDO)       return false;
    if (g_selection[brakeSystem_Q] == BOSCH_B && g_selection[SteeringSystem_Q] != BOSCH_S) return false;
    return true;
}

void runProducedCar()
{
    if (!isValidCombination())
    {
        printf("자동차가 동작되지 않습니다\n");
        return;
    }

    if (g_selection[Engine_Q] == 4)
    {
        printf("엔진이 고장나있습니다.\n");
        printf("자동차가 움직이지 않습니다.\n");
        return;
    }

    switch (g_selection[CarType_Q])
    {
    case SEDAN: printf("Car Type : Sedan\n"); break;
    case SUV:   printf("Car Type : SUV\n");   break;
    case TRUCK: printf("Car Type : Truck\n"); break;
    }
    switch (g_selection[Engine_Q])
    {
    case GM:     printf("Engine : GM\n");     break;
    case TOYOTA: printf("Engine : TOYOTA\n"); break;
    case WIA:    printf("Engine : WIA\n");    break;
    }
    switch (g_selection[brakeSystem_Q])
    {
    case MANDO:       printf("Brake System : Mando\n");       break;
    case CONTINENTAL: printf("Brake System : Continental\n"); break;
    case BOSCH_B:     printf("Brake System : Bosch\n");       break;
    }
    switch (g_selection[SteeringSystem_Q])
    {
    case BOSCH_S: printf("SteeringSystem : Bosch\n"); break;
    case MOBIS:   printf("SteeringSystem : Mobis\n"); break;
    }

    printf("자동차가 동작됩니다.\n");
}

void testProducedCar()
{
    if (g_selection[CarType_Q] == SEDAN && g_selection[brakeSystem_Q] == CONTINENTAL)
    {
        printf("자동차 부품 조합 테스트 결과 : FAIL\n");
        printf("Sedan에는 Continental제동장치 사용 불가\n");
    }
    else if (g_selection[CarType_Q] == SUV && g_selection[Engine_Q] == TOYOTA)
    {
        printf("자동차 부품 조합 테스트 결과 : FAIL\n");
        printf("SUV에는 TOYOTA엔진 사용 불가\n");
    }
    else if (g_selection[CarType_Q] == TRUCK && g_selection[Engine_Q] == WIA)
    {
        printf("자동차 부품 조합 테스트 결과 : FAIL\n");
        printf("Truck에는 WIA엔진 사용 불가\n");
    }
    else if (g_selection[CarType_Q] == TRUCK && g_selection[brakeSystem_Q] == MANDO)
    {
        printf("자동차 부품 조합 테스트 결과 : FAIL\n");
        printf("Truck에는 Mando제동장치 사용 불가\n");
    }
    else if (g_selection[brakeSystem_Q] == BOSCH_B && g_selection[SteeringSystem_Q] != BOSCH_S)
    {
        printf("자동차 부품 조합 테스트 결과 : FAIL\n");
        printf("Bosch제동장치에는 Bosch조향장치 이외 사용 불가\n");
    }
    else
    {
        printf("자동차 부품 조합 테스트 결과 : PASS\n");
    }
}

#endif