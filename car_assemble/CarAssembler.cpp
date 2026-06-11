#include "CarAssembler.h"
#include <cstdio>
#include <windows.h>

enum Step { CarType_S, Engine_S, Brake_S, Steering_S, RunTest_S };

static bool inRange(int v, int lo, int hi) { return v >= lo && v <= hi; }

CarAssembler::CarAssembler(PartRegistry& registry, CombinationValidator& validator)
    : registry_(registry), validator_(validator), step_(CarType_S) {}

void CarAssembler::showMenu() const {
    switch (step_) {
    case CarType_S:  ui_.printCarTypeMenu(registry_);   break;
    case Engine_S:   ui_.printEngineMenu(registry_);    break;
    case Brake_S:    ui_.printBrakeMenu(registry_);     break;
    case Steering_S: ui_.printSteeringMenu(registry_);  break;
    case RunTest_S:
        printf("\033[H\033[2J");
        printf("멋진 차량이 완성되었습니다.\n");
        printf("어떤 동작을 할까요?\n");
        printf("0. 처음 화면으로 돌아가기\n");
        printf("1. RUN\n");
        printf("2. Test\n");
        break;
    }
    printf("===============================\n");
}

bool CarAssembler::handleInput(int answer) {
    if (answer == -1) return false;  // exit

    if (answer == -2) {
        printf("ERROR :: 숫자만 입력 가능\n");
        Sleep(800);
        return true;
    }

    if (step_ == CarType_S && !inRange(answer, 1, registry_.carTypeCount())) {
        printf("ERROR :: 1 ~ %d 범위만 선택 가능\n", registry_.carTypeCount());
        Sleep(800);
        return true;
    }
    if (step_ == Engine_S && !inRange(answer, 0, registry_.engineCount())) {
        printf("ERROR :: 0 ~ %d 범위만 선택 가능\n", registry_.engineCount());
        Sleep(800);
        return true;
    }
    if (step_ == Brake_S && !inRange(answer, 0, registry_.brakeSystemCount())) {
        printf("ERROR :: 0 ~ %d 범위만 선택 가능\n", registry_.brakeSystemCount());
        Sleep(800);
        return true;
    }
    if (step_ == Steering_S && !inRange(answer, 0, registry_.steeringSystemCount())) {
        printf("ERROR :: 0 ~ %d 범위만 선택 가능\n", registry_.steeringSystemCount());
        Sleep(800);
        return true;
    }
    if (step_ == RunTest_S && !inRange(answer, 0, 2)) {
        printf("ERROR :: Run 또는 Test 중 하나를 선택 필요\n");
        Sleep(800);
        return true;
    }

    // 처음으로
    if (answer == 0 && step_ == RunTest_S) { step_ = CarType_S; return true; }
    // 이전으로
    if (answer == 0 && step_ > 0)          { step_--;           return true; }

    switch (step_) {
    case CarType_S:
        car_.setType(registry_.getCarType(answer));
        Sleep(800);
        step_ = Engine_S;
        break;
    case Engine_S:
        car_.setEngine(registry_.getEngine(answer));
        Sleep(800);
        step_ = Brake_S;
        break;
    case Brake_S:
        car_.setBrakeSystem(registry_.getBrakeSystem(answer));
        Sleep(800);
        step_ = Steering_S;
        break;
    case Steering_S:
        car_.setSteeringSystem(registry_.getSteeringSystem(answer));
        Sleep(800);
        step_ = RunTest_S;
        break;
    case RunTest_S:
        if (answer == 1) { runCar();  Sleep(2000); }
        if (answer == 2) { printf("Test...\n"); Sleep(1500); testCar(); Sleep(2000); }
        break;
    }
    return true;
}

void CarAssembler::runCar() const {
    if (!validator_.isValid(car_)) {
        printf("자동차가 동작되지 않습니다\n");
        return;
    }
    ui_.printRunResult(car_);
}

void CarAssembler::testCar() const {
    bool valid = validator_.isValid(car_);
    ui_.printTestResult(valid, validator_.getFailReason(car_));
}

void CarAssembler::run() {
    while (true) {
        showMenu();
        int answer = ui_.getInput();
        if (!handleInput(answer)) break;
    }
}
