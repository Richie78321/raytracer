#pragma once
#include "scene.h"

namespace rt {
  class Plane : public virtual SceneObject {
  public:
    Plane(float3 normal, float3 point, Material& material);
    virtual RayIntersection getIntersection(const Ray& ray) const;
    virtual const Material& getMaterial() const;

    Material& material;
    float3 normal;
    float3 point;
  };
}