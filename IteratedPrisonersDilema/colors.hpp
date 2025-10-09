#pragma once

#include <ostream>

class RGBColor {
public:
  RGBColor(int r, int g, int b) : r(r), g(g), b(b) {}
  RGBColor lerp(RGBColor other, double t) const {
    double r = static_cast<double>(this->r);
    double g = static_cast<double>(this->g);
    double b = static_cast<double>(this->b);
    return RGBColor(static_cast<int>(r + (other.r - r) * t),
                    static_cast<int>(g + (other.g - g) * t),
                    static_cast<int>(b + (other.b - b) * t));
  }
  int r;
  int g;
  int b;
};

inline std::ostream& operator<<(std::ostream& os, const RGBColor& color) {
  os << "\033[38;2;" << color.r << ";" << color.g << ";" << color.b << "m";
  return os;
}