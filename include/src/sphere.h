#include <linalg.h>
#include "scene.h"

namespace rt {
  class Sphere : public virtual SceneObject {
  public:
    Sphere(float3 center, float radius);
    virtual RayIntersection getIntersection(const Ray& ray) const;

    float3 center;
    float radius;
  };
}