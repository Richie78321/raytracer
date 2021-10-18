#include "scene.h"

namespace rt {
  class Plane : public virtual SceneObject {
  public:
    Plane(float3 normal, float3 point, SceneColor color = SCENE_WHITE, bool reflective = false);
    virtual RayIntersection getIntersection(const Ray& ray) const;
    virtual bool isReflective() const;
    virtual SceneColor getColor() const;

    SceneColor color;
    bool reflective;
    float3 normal;
    float3 point;
  };
}