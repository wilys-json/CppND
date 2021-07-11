#ifndef FOOD_H
#define FOOD_H

#include "SDL.h"
#include "GameObject.h"
#include "SwitchableColor.h"

class Food : public GameObject, public SwitchableColor{

  public:
    // enum Color { kRed, kYellow, kGreen, kBlue, kPurple, kWhite };
    // enum BlinkState { kOn, kOff };
    enum State { kNormal, kSuper, kSpeedup, kPoison };
    Food() {};
    ~Food() {};
    Food(int grid_width, int grid_height) :
     GameObject(grid_width, grid_height) {};
    void Update();
    // Color getColor() const { return color; };
    void setState(State new_state) { state = new_state; };
    State getState() const { return state; };
  private:
    // Color color = Color::kYellow;
    // BlinkState blinkstate = BlinkState::kOff;
    State state = State::kNormal;
    void Blink() override;
    void Poisoned();
    void turnSpeedUp();
};

#endif
