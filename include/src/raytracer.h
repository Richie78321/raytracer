#include <memory>
#include <vector>
#include <linalg.h>

namespace rt {
  using linalg::aliases::float3;
  using linalg::aliases::float4;

  struct SceneCamera {
    float3 position;
    float4 rotation;
    float fov;
    int resolution;
  };

  struct RayIntersection;
  struct Ray;

  class SceneObject {
  public:
    virtual RayIntersection getIntersection(const Ray& ray) const = 0;
  };

  class Sphere : public virtual SceneObject {
  public:
    Sphere(float3 center, float radius);
    virtual RayIntersection getIntersection(const Ray& ray) const;

    float3 center;
    float radius;
  };
  
  struct Ray {
    float3 startPosition;
    float3 direction;

    RayIntersection getClosestIntersection(const std::vector<std::shared_ptr<SceneObject>>& sceneObjects) const;
    int getRayColor(const std::vector<std::shared_ptr<SceneObject>>& sceneObjects) const;
  };

  struct RayIntersection {
    bool intersected;
    float3 intersectionPosition;
    float3 surfaceNormal;
    std::shared_ptr<SceneObject> objectIntersected;
  };
  
  class Scene {
  public:
    Scene(std::vector<std::shared_ptr<SceneObject>> sceneObjects);
    std::vector<int> renderScene(const SceneCamera& camera) const;
  private:
    std::vector<std::shared_ptr<SceneObject>> sceneObjects;
  };
}