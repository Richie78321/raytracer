#include <algorithm>
#include <thread>
#include "scene.h"

namespace rt {
  constexpr const int RAYTRACE_THREADS = 10;

  Scene::Scene(std::vector<std::shared_ptr<SceneObject>> sceneObjects, std::vector<Light> lights) : sceneObjects(sceneObjects), lights(lights) {}

  std::vector<int> Scene::renderScene(const SceneCamera& camera) const {
    float3 cameraUp = linalg::qzdir(camera.rotation);
    float3 cameraRight = linalg::qxdir(camera.rotation);
    float3 cameraForward = linalg::qydir(camera.rotation);

    float pixelRayIncrement = tan(camera.fov) / camera.resolution;
    float halfResolution = static_cast<float>(camera.resolution) / 2;
    std::vector<int> scene(camera.resolution * camera.resolution);

    std::vector<std::thread> raytrace_threads;
    int size_increment = (int)ceil((float)scene.size() / RAYTRACE_THREADS);
    for (int i = 0; i < RAYTRACE_THREADS; i++) {
      int start = std::min(i * size_increment, (int)scene.size());
      int end = std::min((i + 1) * size_increment, (int)scene.size());

      raytrace_threads.emplace_back([&, start, end]() {
        for (int x = start; x < end; x++) {
          int i = x / camera.resolution;
          int j = x % camera.resolution;
          Ray pixelRay{ camera.position, linalg::normalize(cameraForward + ((j - halfResolution) * pixelRayIncrement * cameraRight) + ((i - halfResolution) * pixelRayIncrement * cameraUp)) };
          scene[x] = pixelRay.getRayColor(*this).toHex();
        }
      });
    }

    for (auto& thread : raytrace_threads) {
      thread.join();
    }

    return scene;
  }

  const std::vector<std::shared_ptr<SceneObject>>& Scene::getSceneObjects() const {
    return this->sceneObjects;
  }

  const std::vector<Light>& Scene::getLights() const {
    return this->lights;
  }

  RayIntersection Ray::getClosestIntersection(const std::vector<std::shared_ptr<SceneObject>>& sceneObjects) const {
    RayIntersection closestIntersection { false };
    float intersectionDistance = 0;

    for (auto sceneObject : sceneObjects) {
      RayIntersection intersection = sceneObject->getIntersection(*this);
      if (intersection.intersected) {
        float newIntersectionDistance = linalg::length(intersection.intersectionPosition - this->startPosition);
        if (!closestIntersection.intersected || newIntersectionDistance < intersectionDistance) {
          closestIntersection = intersection;
          intersectionDistance = newIntersectionDistance;
        }
      }
    }

    return closestIntersection;
  }

  SceneColor Ray::getRayColor(const Scene& scene) const {
    RayIntersection closestIntersection = this->getClosestIntersection(scene.getSceneObjects());
    if (!closestIntersection.intersected) {
      return SCENE_BLACK;
    }

    // TODO: assumming everything opaque for now
    SceneColor objectColor = SCENE_BLACK;
    for (auto& light : scene.getLights()) {
      // Move the ray start slightly towards the target light to stop the ray from intersecting
      // with the surface that it is originating from.
      float3 shadowRayDirection = linalg::normalize(light.position - closestIntersection.intersectionPosition);
      RayIntersection shadowRayIntersection = Ray{ closestIntersection.intersectionPosition + (shadowRayDirection * 0.01f), shadowRayDirection }.getClosestIntersection(scene.getSceneObjects());

      // If the shadow ray did intersect something, determine if it's behind the light source.
      if (!shadowRayIntersection.intersected || linalg::dot(shadowRayIntersection.intersectionPosition - light.position, shadowRayDirection) > 0) {
        objectColor = objectColor + (light.getColorAtPosition(closestIntersection.intersectionPosition) * std::max(linalg::dot(shadowRayDirection, closestIntersection.surfaceNormal), 0.0f));
      }
    }

    return objectColor;
  }
}