#include <iostream>
#include <memory>
#include <string>
#include <chrono>
#include <linalg.h>
#include "raylib.h"
#include "raytracer.h"

using namespace rt;

constexpr float MOUSE_SENSITIVITY = 2.5f;
constexpr float SPEED = 0.5f;

int freeValue = 0;
int freeValue2 = 0;

float4 fromAxisAngle(float3 axis, float angle) {
  float factor = sin(angle / 2);
  return float4{ factor * axis.x, factor * axis.y, factor * axis.z, (float)cos(angle / 2) };
}

void cameraControl(int screenSize, SceneCamera& camera) {
  int mouseDeltaX = GetMouseX() - (screenSize / 2);
  int mouseDeltaY = GetMouseY() - (screenSize / 2);

  if (mouseDeltaX != 0) {
    camera.rotation = linalg::qmul(fromAxisAngle(float3{ 0, 0, 1 }, (float)(mouseDeltaX * MOUSE_SENSITIVITY / screenSize)), camera.rotation);
  }

  if (mouseDeltaY != 0) {
    camera.rotation = linalg::qmul(camera.rotation, fromAxisAngle(float3{ 1, 0, 0 }, (float)(-mouseDeltaY * MOUSE_SENSITIVITY / screenSize)));
  }

  SetMousePosition(screenSize / 2, screenSize / 2);

  if (IsKeyDown(KEY_E)) {
    camera.position.z -= SPEED;
  }

  if (IsKeyDown(KEY_Q)) {
    camera.position.z += SPEED;
  }

  if (IsKeyDown(KEY_W) || IsKeyDown(KEY_S)) {
    float3 forward = linalg::qydir(camera.rotation);
    forward.z = 0;
    forward = linalg::normalize(forward) * SPEED;

    if (IsKeyDown(KEY_W)) {
      camera.position += forward;
    }

    if (IsKeyDown(KEY_S)) {
      camera.position -= forward;
    }
  }

  if (IsKeyDown(KEY_A) || IsKeyDown(KEY_D)) {
    float3 right = linalg::qxdir(camera.rotation);
    right.z = 0;
    right = linalg::normalize(right) * SPEED;

    if (IsKeyDown(KEY_A)) {
      camera.position -= right;
    }

    if (IsKeyDown(KEY_D)) {
      camera.position += right;
    }
  }

  if (IsKeyPressed(KEY_UP)) {
    freeValue++;
  }

  if (IsKeyPressed(KEY_DOWN)) {
    freeValue--;
  }

  if (IsKeyPressed(KEY_RIGHT)) {
    freeValue2++;
  }

  if (IsKeyPressed(KEY_LEFT)) {
    freeValue2--;
  }
}

int main() {
  StaticMaterial sphere1Material = StaticMaterial{SceneColor{168,169,173}, 0.0f};
  StaticMaterial sphere2Material = StaticMaterial{SceneColor{ 255, 0, 0 }, 0.0f};
  StaticMaterial planeMaterial = StaticMaterial{SCENE_WHITE, 0.0f};
  std::shared_ptr<Sphere> sphere1 = std::make_shared<Sphere>(float3{ 5, 14, 0 }, 4, sphere1Material);
  std::shared_ptr<Sphere> sphere2 = std::make_shared<Sphere>(float3{ -5, 14, 0 }, 4, sphere2Material);
  std::shared_ptr<Plane> plane = std::make_shared<Plane>(float3{ 0, 0, -1 }, float3{ 0, 0, 4 }, planeMaterial);
  Scene scene {{ sphere1, sphere2, plane }, {
    // Light{float3{0, 2, -10}, 1000, SceneColor{255, 0, 0}},
    Light{float3{0, -2, -10}, 1000, SCENE_WHITE},
    // Light{float3{0, 0, -1000}, 100000, SCENE_WHITE},
  }};
  SceneCamera camera { float3{ 0, 0, 0 }, float4{ 0, 1, 0, 0 }, 60 * DEG2RAD, 250 };

  const int screenSize = 800;

  InitWindow(screenSize, screenSize, "Raytracer");
  HideCursor();
  // SetTargetFPS(30);

  RenderTexture2D renderTarget = LoadRenderTexture(camera.resolution, camera.resolution);

  while (!WindowShouldClose()) {
    // auto render_start = std::chrono::high_resolution_clock::now();
    std::vector<int> render = scene.renderScene(camera);
    // auto render_end = std::chrono::high_resolution_clock::now();
    // std::cout << "Render Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(render_end - render_start).count() << "\n";

    cameraControl(screenSize, camera);
    
    // auto texture_draw_start = std::chrono::high_resolution_clock::now();
    BeginTextureMode(renderTarget);
    ClearBackground(BLACK);
    for (int i = 0; i < render.size(); i++) {
      DrawPixel(i % camera.resolution, i / camera.resolution, GetColor(render[i]));
    }
    EndTextureMode();
    // auto texture_draw_end = std::chrono::high_resolution_clock::now();
    // std::cout << "Texture Draw Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(texture_draw_end - texture_draw_start).count() << "\n";

    // auto texture_display_start = std::chrono::high_resolution_clock::now();
    BeginDrawing();
    ClearBackground(BLACK);
    DrawTextureEx(renderTarget.texture, { 0, 0 }, 0, (float)screenSize / camera.resolution, WHITE);
    DrawText(std::to_string(GetFPS()).c_str(), 0, 0, 30, RAYWHITE);
    EndDrawing();
    // auto texture_display_end = std::chrono::high_resolution_clock::now();
    // std::cout << "Texture Display Time: " << std::chrono::duration_cast<std::chrono:milliseconds>(texture_display_end - texture_display_start).count() << "\n\n";

    sphere1Material.reflectivity = freeValue / 10.0f;
    sphere2Material.reflectivity = freeValue2 / 10.0f;
  }
  
  return 0;
}
