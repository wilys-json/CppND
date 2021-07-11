#ifndef SWITCHABLECOLOR_H
#define SWITCHABLECOLOR_H

class SwitchableColor {
  public:
    enum Color { kRed, kYellow, kGreen, kBlue, kPurple, kWhite };
    SwitchableColor() {};
    virtual ~SwitchableColor() {};
    Color getColor() const { return color; };
  protected:
    virtual void Blink() = 0;
    Color color;
};

#endif