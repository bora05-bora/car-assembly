#include "CarUI.h"
#include "PartRegistry.h"
#include "Car.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

#define CLEAR_SCREEN "\033[H\033[2J"

void CarUI::printCarTypeMenu(const PartRegistry& registry) const {
    printf(CLEAR_SCREEN);
    printf("        ______________\n");
    printf("       /|            | \n");
    printf("  ____/_|_____________|____\n");
    printf(" |                      O  |\n");
    printf(" '-(@)----------------(@)--'\n");
    printf("===============================\n");
    printf("어떤 차량 타입을 선택할까요?\n");
    registry.printCarTypes();
}

void CarUI::printEngineMenu(const PartRegistry& registry) const {
    printf(CLEAR_SCREEN);
    printf("어떤 엔진을 탑재할까요?\n");
    printf("0. 뒤로가기\n");
    registry.printEngines();
}

void CarUI::printBrakeMenu(const PartRegistry& registry) const {
    printf(CLEAR_SCREEN);
    printf("어떤 제동장치를 선택할까요?\n");
    printf("0. 뒤로가기\n");
    registry.printBrakeSystems();
}

void CarUI::printSteeringMenu(const PartRegistry& registry) const {
    printf(CLEAR_SCREEN);
    printf("어떤 조향장치를 선택할까요?\n");
    printf("0. 뒤로가기\n");
    registry.printSteeringSystems();
}

void CarUI::printRunResult(const Car& car) const {
    if (car.getEngine()->isBroken()) {
        printf("엔진이 고장나있습니다.\n");
        printf("자동차가 움직이지 않습니다.\n");
        return;
    }
    printf("Car Type : %s\n",      car.getType()->getName().c_str());
    printf("Engine : %s\n",        car.getEngine()->getName().c_str());
    printf("Brake System : %s\n",  car.getBrakeSystem()->getName().c_str());
    printf("SteeringSystem : %s\n",car.getSteeringSystem()->getName().c_str());
    printf("자동차가 동작됩니다.\n");
}

void CarUI::printTestResult(bool isValid, const std::string& reason) const {
    if (isValid) {
        printf("자동차 부품 조합 테스트 결과 : PASS\n");
    } else {
        printf("자동차 부품 조합 테스트 결과 : FAIL\n");
        printf("%s\n", reason.c_str());
    }
}

int CarUI::getInput() const {
    char buf[100];
    printf("INPUT > ");
    if (!fgets(buf, sizeof(buf), stdin)) return -1;
    buf[strcspn(buf, "\r\n")] = '\0';

    if (!strcmp(buf, "exit")) return -1;

    char* endptr;
    int answer = (int)strtol(buf, &endptr, 10);
    if (*endptr != '\0') return -2;  // 숫자 아님
    return answer;
}
