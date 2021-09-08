#ifndef FOOD_H
#define FOOD_H

#include <memory>
#include "SDL.h"
#include "GameObject.h"
#include "SwitchableColor.h"

class Food : public GameObject, public SwitchableColor{

  public:
    enum class State { kNormal, kSuper, kSpeedup, kPoison };

    // Constructors & Destructors
    Food() {};
    ~Food() {};
    Food(int grid_width, int grid_height, std::shared_ptr<Map<GameObject>> gameMap) :
      GameObject(grid_width, grid_height, gameMap),
      SwitchableColor() {};

    // Implement GameObject
    void Initialize() override;
    void Initialize(const int& score, const int& x, const int& y); // overload
    void Update() override;

    // Setter
    void setState(State new_state) { state = new_state; };

    // Getter
    State getState() const { return state; };

  private:
    // Attribute
    State state {State::kNormal};

    // Implement SwitchableColor
    void Blink() override;

    // Methods
    void Poisoned();
    void turnSpeedUp();
};

#endif
