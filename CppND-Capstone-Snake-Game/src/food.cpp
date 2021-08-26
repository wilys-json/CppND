#include "food.h"

void Food::Initialize() {
  if (!initialized) {
    projectToMap();
    initialized = true;
  }
}

void Food::Initialize(const int& score, const int& x, const int& y) {
  Initialize();
  if (score > 0 && score % 5 == 0) setState(Food::State::kSuper);
  if (x % 5 == 0 && y % 5 != 0) {
    setState(Food::State::kPoison);
  }
  if (x % 5 == 0 && y % 5 == 0) {
    setState(Food::State::kSpeedup);
  }
}

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
  projectToMap();
}

void Food::Blink() {
  color = (color == Color::kYellow) ? Color::kWhite : Color::kYellow;
}

void Food::Poisoned() {
  color = (color == Color::kPurple) ? Color::kGreen : Color::kPurple;
}

void Food::turnSpeedUp() {
  color = (color == Color::kRed) ? Color::kWhite : Color::kRed;
}
