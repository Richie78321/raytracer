#pragma once
#include "light.h"

namespace rt {
  class Material {
  public:
    virtual SceneColor getColor() const = 0;
    virtual float getReflectivity() const = 0;
  };

  class StaticMaterial : public virtual Material {
  public:
    StaticMaterial(SceneColor color, float reflectivity);
    virtual SceneColor getColor() const;
    virtual float getReflectivity() const;
    SceneColor color;
    float reflectivity;
  };
}