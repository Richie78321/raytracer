#include "scene.h"

namespace rt {
  class Plane : public virtual SceneObject {
  public:
    Plane(float3 normal, float3 point, bool reflective = false);
    virtual RayIntersection getIntersection(const Ray& ray) const;
    virtual bool isReflective() const;

    bool reflective;
    float3 normal;
    float3 point;
  };
}