#include "sphere.h"

namespace rt {
  Sphere::Sphere(float3 center, float radius, Material& material) : center(center), radius(radius), material(material) {}

  const Material& Sphere::getMaterial() const {
    return this->material;
  }

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
      // Two points
      float sqrtDeterminant = sqrt(determinant);
      float3 intersectionPoint = (float)((-b - sqrtDeterminant) / (2 * a)) * ray.direction + ray.startPosition;
      if (linalg::dot(intersectionPoint - ray.startPosition, ray.direction) < 0) {
        // Closer intersection point is behind the camera, check the farther intersection point.
        intersectionPoint = (float)((-b + sqrtDeterminant) / (2 * a)) * ray.direction + ray.startPosition;
        if (linalg::dot(intersectionPoint - ray.startPosition, ray.direction) < 0) {
          return { false };
        }
      }

      return { true, intersectionPoint, linalg::normalize(intersectionPoint - this->center), this };
    }
  }
}