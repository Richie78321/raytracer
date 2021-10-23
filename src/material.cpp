#include "material.h"

namespace rt {
  StaticMaterial::StaticMaterial(SceneColor color, float reflectivity) : color(color), reflectivity(reflectivity) {}

  SceneColor StaticMaterial::getColor() const {
    return this->color;
  }

  float StaticMaterial::getReflectivity() const {
    return this->reflectivity;
  }
}