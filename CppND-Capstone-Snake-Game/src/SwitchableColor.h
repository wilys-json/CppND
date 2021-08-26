#ifndef SWITCHABLECOLOR_H
#define SWITCHABLECOLOR_H

enum class Color { kRed, kYellow, kGreen, kBlue, kPurple, kWhite, kGrey};

namespace DefaultColors {
  static const Color RivalHeadColor{Color::kGrey};
  static const Color RivalBodyColor{Color::kRed};
  static const Color PlayerHeadColor{Color::kBlue};
  static const Color PlayerBodyColor{Color::kWhite};
};

// Interface for Blinking effect
class SwitchableColor {
  public:
    SwitchableColor() {};
    virtual ~SwitchableColor() {};
    Color& getColor() { return color; }
  protected:
    virtual void Blink() = 0;
    Color color{Color::kYellow};
};



#endif
