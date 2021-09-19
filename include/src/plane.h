#include "scene.h"

namespace rt {
  class Plane : public virtual SceneObject {
  public:
    Plane(float3 normal, float3 point);
    virtual RayIntersection getIntersection(const Ray& ray) const;

    float3 normal;
    float3 point;
  };
}