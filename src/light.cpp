#include "light.h"
#include <linalg.h>
#include <iostream>

namespace rt {
  SceneColor::SceneColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : r(r), g(g), b(b), a(a) {}
  
  SceneColor::SceneColor(int hex) : r((hex >> 24) & 0xFF), g((hex >> 16) & 0xFF), b((hex >> 8) & 0xFF), a(hex & 0xFF) {}

  SceneColor SceneColor::operator*(float intensity) const {
    return SceneColor{ (unsigned char)floor(this->r * intensity), (unsigned char)floor(this->g * intensity), (unsigned char)floor(this->b * intensity), a };
  }

  int SceneColor::toHex() const {
    return (r << 24) | (g << 16) | (b << 8) | a;
  }

  SceneColor Light::getColorAtPosition(float3 position) const {
    return this->color * (intensity / linalg::length2(position - this->position));
  }
}