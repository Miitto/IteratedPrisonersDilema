#pragma once

class RGBColor {
public:
  RGBColor(int r, int g, int b) : r(r), g(g), b(b) {}
  RGBColor lerp(RGBColor other, double t) const {
    return RGBColor(r + (other.r - r) * t, g + (other.g - g) * t,
                    b + (other.b - b) * t);
  }
  int r;
  int g;
  int b;
};