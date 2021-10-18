#include <linalg.h>
#include "scene.h"

namespace rt {
  class Sphere : public virtual SceneObject {
  public:
    Sphere(float3 center, float radius, SceneColor color = SCENE_WHITE, bool reflective = false);
    virtual RayIntersection getIntersection(const Ray& ray) const;
    virtual bool isReflective() const;
    virtual SceneColor getColor() const;

    SceneColor color;
    bool reflective;
    float3 center;
    float radius;
  };
}