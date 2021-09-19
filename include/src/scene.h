#pragma once

#include <memory>
#include <vector>
#include <linalg.h>
#include "light.h"

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

  struct RayIntersection {
    bool intersected;
    float3 intersectionPosition;
    float3 surfaceNormal;
    const SceneObject* objectIntersected;
  };
  
  class Scene {
  public:
    Scene(std::vector<std::shared_ptr<SceneObject>> sceneObjects, std::vector<Light> lights);
    std::vector<int> renderScene(const SceneCamera& camera) const;

    const std::vector<std::shared_ptr<SceneObject>>& getSceneObjects() const;
    const std::vector<Light>& getLights() const;
  private:
    std::vector<std::shared_ptr<SceneObject>> sceneObjects;
    std::vector<Light> lights;
  };

  struct Ray {
    float3 startPosition;
    float3 direction;

    RayIntersection getClosestIntersection(const std::vector<std::shared_ptr<SceneObject>>& sceneObjects) const;
    SceneColor getRayColor(const Scene& scene) const;
  };
}