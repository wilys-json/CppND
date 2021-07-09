#include "food.h"


void Food::Update() {
  switch(state) {
    case State::kSuper:
      Blink();
      break;
    case State::kPoison:
      Poisoned();
      break;
    case State::kSpeedup:
      turnSpeedUp();
      break;
    case State::kNormal:
      color = Color::kYellow;
      break;
  }
}

void Food::Blink() {
  blinkstate = (blinkstate == BlinkState::kOff) ? BlinkState::kOn : BlinkState::kOff;
  color = (blinkstate == BlinkState::kOff) ? Color::kWhite : Color::kYellow;
}

void Food::Poisoned() {
  color = Color::kPurple;
}

void Food::turnSpeedUp() {
  blinkstate = (blinkstate == BlinkState::kOff) ? BlinkState::kOn : BlinkState::kOff;
  color = (blinkstate == BlinkState::kOff) ? Color::kWhite : Color::kRed;
}
