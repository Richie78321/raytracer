#include <linalg.h>
#include "scene.h"

namespace rt {
  class Sphere : public virtual SceneObject {
  public:
    Sphere(float3 center, float radius, bool reflective = false);
    virtual RayIntersection getIntersection(const Ray& ray) const;
    virtual bool isReflective() const;

    bool reflective;
    float3 center;
    float radius;
  };
}