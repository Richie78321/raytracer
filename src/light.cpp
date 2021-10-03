#include "light.h"
#include <linalg.h>
#include <iostream>
#include <algorithm>

namespace rt {
  unsigned char clampCharAdd(unsigned char val1, unsigned char val2) {
    return (unsigned char)std::min(std::max((int)val1 + (int)val2, 0), 255);
  }

  SceneColor::SceneColor(unsigned char r, unsigned char g, unsigned char b) : r(r), g(g), b(b), a(255) {}

  SceneColor::SceneColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : r(r), g(g), b(b), a(a) {}
  
  SceneColor::SceneColor(int hex) : r((hex >> 24) & 0xFF), g((hex >> 16) & 0xFF), b((hex >> 8) & 0xFF), a(hex & 0xFF) {}

  SceneColor SceneColor::operator*(float intensity) const {
    return SceneColor{ (unsigned char)floor(this->r * intensity), (unsigned char)floor(this->g * intensity), (unsigned char)floor(this->b * intensity), a };
  }

  SceneColor SceneColor::operator+(SceneColor other) const {
    return SceneColor{ clampCharAdd(this->r, other.r), clampCharAdd(this->g, other.g), clampCharAdd(this->b, other.b), clampCharAdd(this->a, other.a) };
  }

  int SceneColor::toHex() const {
    return (r << 24) | (g << 16) | (b << 8) | a;
  }

  SceneColor Light::getColorAtPosition(float3 position) const {
    return this->color * std::min((intensity / linalg::length2(position - this->position)), 1.0f);
  }
}