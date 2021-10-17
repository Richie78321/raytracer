#include "plane.h"

namespace rt {
  Plane::Plane(float3 normal, float3 point, bool reflective) : normal(normal), point(point), reflective(reflective) {}

  bool Plane::isReflective() const {
    return this->reflective;
  }

  RayIntersection Plane::getIntersection(const Ray& ray) const {
    float rayDotNormal = linalg::dot(ray.direction, this->normal);

    if (fabs(rayDotNormal) < 0.01f) {
      return { false };
    }

    float intersectionPointMult = (linalg::dot(this->normal, this->point) - linalg::dot(this->normal, ray.startPosition)) / rayDotNormal;
    if (intersectionPointMult < 0) {
      // Intersection is behind the start of the ray.
      return { false };
    }

    return { true, intersectionPointMult * ray.direction + ray.startPosition, this->normal, this };
  }
}