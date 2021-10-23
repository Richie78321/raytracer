#pragma once

#include <memory>
#include <vector>
#include <linalg.h>
#include "light.h"
#include "material.h"

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
    virtual const Material& getMaterial() const = 0;
  };

  struct RayIntersection {
    bool intersected;
    float3 intersectionPosition;
    float3 surfaceNormal;
    const SceneObject* objectIntersected;
  };
  
  class Scene {
  public:
    Scene(std::vector<std::shared_ptr<SceneObject>> sceneObjects, std::vector<Light> lights, SceneColor ambientColor = SKY_BLUE);
    std::vector<int> renderScene(const SceneCamera& camera) const;

    const SceneColor ambientColor;
    const std::vector<std::shared_ptr<SceneObject>>& getSceneObjects() const;
    const std::vector<Light>& getLights() const;
  private:
    std::vector<std::shared_ptr<SceneObject>> sceneObjects;
    std::vector<Light> lights;
  };

  constexpr const int MAX_RAY_BOUNCES = 5;

  struct Ray {
  public:
    float3 startPosition;
    float3 direction;

    RayIntersection getClosestIntersection(const std::vector<std::shared_ptr<SceneObject>>& sceneObjects) const;
    SceneColor getRayColor(const Scene& scene, int bounces = MAX_RAY_BOUNCES) const;
    
  private:
    SceneColor getOpaqueColor(const Scene& scene, RayIntersection intersection) const;
    SceneColor getReflectiveColor(const Scene& scene, RayIntersection intersection, int bounces) const;
  };
}