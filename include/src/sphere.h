#pragma once
#include <linalg.h>
#include "scene.h"

namespace rt {
  class Sphere : public virtual SceneObject {
  public:
    Sphere(float3 center, float radius, Material& material);
    virtual RayIntersection getIntersection(const Ray& ray) const;
    virtual const Material& getMaterial() const;

    Material& material;
    float3 center;
    float radius;
  };
}