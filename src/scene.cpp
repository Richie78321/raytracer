#include <algorithm>
#include <thread>
#include "scene.h"

namespace rt {
  constexpr const int RAYTRACE_THREADS = 10;

  Scene::Scene(std::vector<std::shared_ptr<SceneObject>> sceneObjects, std::vector<Light> lights, SceneColor ambientColor) : sceneObjects(sceneObjects), lights(lights), ambientColor(ambientColor) {}

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

  constexpr const float NUDGE_LENGTH = 0.01f;

  SceneColor Ray::getOpaqueColor(const Scene& scene, RayIntersection intersection) const {
    SceneColor objectColor = SCENE_BLACK;
    for (auto& light : scene.getLights()) {
      // Move the ray start slightly towards the target light to stop the ray from intersecting
      // with the surface that it is originating from.
      float3 shadowRayDirection = linalg::normalize(light.position - intersection.intersectionPosition);
      RayIntersection shadowRayIntersection = Ray{ intersection.intersectionPosition + (shadowRayDirection * NUDGE_LENGTH), shadowRayDirection }.getClosestIntersection(scene.getSceneObjects());

      // If the shadow ray did intersect something, determine if it's behind the light source.
      if (!shadowRayIntersection.intersected || linalg::dot(shadowRayIntersection.intersectionPosition - light.position, shadowRayDirection) > 0) {
        objectColor = objectColor + (light.getColorAtPosition(intersection.intersectionPosition) * std::max(linalg::dot(shadowRayDirection, intersection.surfaceNormal), 0.0f));
      }
    }

    return intersection.objectIntersected->getColor() * objectColor;
  }

  SceneColor Ray::getReflectiveColor(const Scene& scene, RayIntersection intersection, int bounces) const {
    float3 reflectionRayDirection = this->direction - (linalg::dot(this->direction, intersection.surfaceNormal) * intersection.surfaceNormal * 2);
    return intersection.objectIntersected->getColor() * Ray{ intersection.intersectionPosition + (reflectionRayDirection * NUDGE_LENGTH), reflectionRayDirection }.getRayColor(scene, bounces - 1);
  }

  SceneColor Ray::getRayColor(const Scene& scene, int bounces) const {
    if (bounces <= 0) {
      return SCENE_BLACK;
    }

    RayIntersection intersection = this->getClosestIntersection(scene.getSceneObjects());
    if (!intersection.intersected) {
      return scene.ambientColor;
    }

    // TODO: Add more modes / variety here
    return intersection.objectIntersected->isReflective() ? getReflectiveColor(scene, intersection, bounces) : getOpaqueColor(scene, intersection);
  }
}