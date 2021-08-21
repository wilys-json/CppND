#ifndef FOOD_H
#define FOOD_H

#include <memory>
#include "SDL.h"
#include "GameObject.h"
#include "SwitchableColor.h"

class Food : public GameObject, public SwitchableColor{

  public:
    enum class State { kNormal, kSuper, kSpeedup, kPoison };
    Food() {};
    ~Food() {};
    Food(int grid_width, int grid_height, std::shared_ptr<Map> gameMap) :
      GameObject(grid_width, grid_height, gameMap),
      SwitchableColor() {};
    void Initialize() override;
    void Update() override;
    void setState(State new_state) { state = new_state; };
    State getState() const { return state; };
  private:
    State state {State::kNormal};
    void Blink() override;
    void Poisoned();
    void turnSpeedUp();
};

#endif
