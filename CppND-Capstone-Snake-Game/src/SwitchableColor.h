#ifndef SWITCHABLECOLOR_H
#define SWITCHABLECOLOR_H

enum class Color { kRed, kYellow, kGreen, kBlue, kPurple, kWhite, kGrey};

namespace DefaultColors {
  static const Color RivalHeadColor{Color::kRed};
  static const Color RivalBodyColor{Color::kGrey};
  static const Color PlayerHeadColor{Color::kBlue};
  static const Color PlayerBodyColor{Color::kWhite};
};

// Interface for Blinking effect
class SwitchableColor {
  public:
    SwitchableColor() {};
    Color& getColor() { return color; }
  protected:
    virtual void Blink() = 0;
    Color color{Color::kYellow};
};



#endif
