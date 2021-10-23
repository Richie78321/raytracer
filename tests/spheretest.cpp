#include <gtest/gtest.h>
#include <linalg.h>
#include "raytracer.h"

using namespace rt;

TEST(SphereIntersection, HandlesNonIntersection) {
  StaticMaterial defaultMaterial = StaticMaterial{SCENE_WHITE, 0.0f};
  Sphere sphere(float3{ 0, 10, 0 }, 4, defaultMaterial);
  Ray ray { float3{ 0, 0, 0 }, float3{ 1, 0, 0 } };

  RayIntersection intersection = sphere.getIntersection(ray);

  ASSERT_FALSE(intersection.intersected);
}

TEST(SphereIntersection, HandlesTangentIntersection) {
  StaticMaterial defaultMaterial = StaticMaterial{SCENE_WHITE, 0.0f};
  Sphere sphere(float3{ 0, 10, 0 }, 4, defaultMaterial);
  // Make a ray tangent with the sphere
  Ray ray { float3{ 4, 0, 0 }, float3{ 0, 1, 0 } };

  RayIntersection intersection = sphere.getIntersection(ray);

  ASSERT_TRUE(intersection.intersected);
}

TEST(SphereIntersection, HandlesDoubleIntersection) {
  StaticMaterial defaultMaterial = StaticMaterial{SCENE_WHITE, 0.0f};
  Sphere sphere(float3{ 0, 10, 0 }, 4, defaultMaterial);
  Ray ray { float3{ 1, 0, 0 }, float3{ 0, 1, 0 } };

  RayIntersection intersection = sphere.getIntersection(ray);

  ASSERT_TRUE(intersection.intersected);
}

TEST(SphereIntersection, HandlesCenterIntersection) {
    StaticMaterial defaultMaterial = StaticMaterial{SCENE_WHITE, 0.0f};
  Sphere sphere(float3{ 0, 10, 0 }, 4, defaultMaterial);
  Ray ray { float3{ 0, 0, 0 }, float3{ 0, 1, 0 } };

  RayIntersection intersection = sphere.getIntersection(ray);

  ASSERT_TRUE(intersection.intersected);
}

TEST(SphereIntersection, IgnoresIntersectionsBehindRay) {
    StaticMaterial defaultMaterial = StaticMaterial{SCENE_WHITE, 0.0f};
  Sphere sphere(float3{ 0, -10, 0 }, 4, defaultMaterial);
  Ray ray { float3{ 0, 0, 0 }, float3{ 0, 1, 0 } };

  RayIntersection intersection = sphere.getIntersection(ray);

  ASSERT_FALSE(intersection.intersected);
}