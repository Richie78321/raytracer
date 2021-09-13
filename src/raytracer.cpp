#include <algorithm>
#include "raytracer.h"

namespace rt {
  Scene::Scene(std::vector<std::shared_ptr<SceneObject>> sceneObjects) : sceneObjects(sceneObjects) {}

  std::vector<int> Scene::renderScene(const SceneCamera& camera) const {
    float3 cameraUp = linalg::qzdir(camera.rotation);
    float3 cameraRight = linalg::qxdir(camera.rotation);
    float3 cameraForward = linalg::qydir(camera.rotation);

    float pixelRayIncrement = tan(camera.fov) / camera.resolution;
    float halfResolution = static_cast<float>(camera.resolution) / 2;
    std::vector<int> scene;
    for (int i = 0; i < camera.resolution; i++) {
      for (int j = 0; j < camera.resolution; j++) {
        Ray pixelRay{ camera.position, linalg::normalize(cameraForward + ((j - halfResolution) * pixelRayIncrement * cameraRight) + ((i - halfResolution) * pixelRayIncrement * cameraUp)) };
        scene.push_back(pixelRay.getRayColor(this->sceneObjects));
      }
    }

    return scene;
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

  int Ray::getRayColor(const std::vector<std::shared_ptr<SceneObject>>& sceneObjects) const {
    // TODO: Add functionality here to make a more advanced raytracing engine.
    RayIntersection closestIntersection = this->getClosestIntersection(sceneObjects);
    if (!closestIntersection.intersected) {
      return 0;
    }

    return 0x0000FF - 0x0000FF * std::max(linalg::dot(closestIntersection.surfaceNormal, float3 { 0, 0, -1 }), 0.f);
  }

  Sphere::Sphere(float3 center, float radius) : center(center), radius(radius) {}

  RayIntersection Sphere::getIntersection(const Ray& ray) const {
    // https://en.wikipedia.org/wiki/Line%E2%80%93sphere_intersection
    float3 centerToStart = ray.startPosition - this->center;
    float a = linalg::length2(ray.direction);
    float b = 2 * linalg::dot(ray.direction, centerToStart);
    float c = linalg::length2(centerToStart) - pow(this->radius, 2);

    float determinant = pow(b, 2) - (4 * a * c);
    if (determinant < 0) {
      return { false };
    } else if (determinant < 0.01f) {
      // One point
      float3 intersectionPoint = (-b / (2 * a)) * ray.direction + ray.startPosition;
      if (linalg::dot(intersectionPoint - ray.startPosition, ray.direction) > 0) {
        return { true, intersectionPoint, linalg::normalize(intersectionPoint - this->center), this };
      } else {
        return { false };
      }
    } else {
      float3 intersectionPoint = (float)((-b - sqrt(determinant)) / (2 * a)) * ray.direction + ray.startPosition;

      if (linalg::dot(intersectionPoint - ray.startPosition, ray.direction) < 0) {
        intersectionPoint = (float)((-b + sqrt(determinant)) / (2 * a)) * ray.direction + ray.startPosition;
        if (linalg::dot(intersectionPoint - ray.startPosition, ray.direction) < 0) {
          return { false };
        }
      }

      return { true, intersectionPoint, linalg::normalize(intersectionPoint - this->center), this };
    }
  }
}