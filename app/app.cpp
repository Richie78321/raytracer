#include <iostream>
#include <memory>
#include <string>
#include <chrono>
#include <linalg.h>
#include "raylib.h"
#include "raytracer.h"

using namespace rt;

float4 fromAxisAngle(float3 axis, float angle) {
  float factor = sin(angle / 2);
  return float4{ factor * axis.x, factor * axis.y, factor * axis.z, (float)cos(angle / 2) };
}

void cameraControl(SceneCamera& camera) {
    if (IsKeyPressed(KEY_LEFT)) {
      camera.rotation = linalg::qmul(camera.rotation, fromAxisAngle(float3{ 0, 0, 1 }, (float)(5 * DEG2RAD)));
    }

    if (IsKeyPressed(KEY_RIGHT)) {
      camera.rotation = linalg::qmul(camera.rotation, fromAxisAngle(float3{ 0, 0, 1 }, (float)(-5 * DEG2RAD)));
    }

    if (IsKeyPressed(KEY_UP)) {
      camera.rotation = linalg::qmul(camera.rotation, fromAxisAngle(float3{ 1, 0, 0 }, (float)(5 * DEG2RAD)));
    }

    if (IsKeyPressed(KEY_DOWN)) {
      camera.rotation = linalg::qmul(camera.rotation, fromAxisAngle(float3{ 1, 0, 0 }, (float)(-5 * DEG2RAD)));
    }

    if (IsKeyPressed(KEY_E)) {
      camera.position.z -= 1;
    }

    if (IsKeyPressed(KEY_Q)) {
      camera.position.z += 1;
    }

    if (IsKeyPressed(KEY_W)) {
      camera.position += linalg::qydir(camera.rotation);
    }

    if (IsKeyPressed(KEY_S)) {
      camera.position -= linalg::qydir(camera.rotation);
    }

    if (IsKeyPressed(KEY_A)) {
      camera.position -= linalg::qxdir(camera.rotation);
    }

    if (IsKeyPressed(KEY_D)) {
      camera.position += linalg::qxdir(camera.rotation);
    }
}

int main() {
  std::shared_ptr<Sphere> sphere = std::make_shared<Sphere>(float3{ 0, 10, 0 }, 4);
  std::shared_ptr<Plane> plane = std::make_shared<Plane>(float3{ 0, 0, -1 }, float3{ 0, 0, 4 });
  Scene scene {{ sphere, plane }};
  SceneCamera camera { float3{ 0, 0, 0 }, float4{ 0, 1, 0, 0 }, 60 * DEG2RAD, 250 };

  const int screenSize = 800;

  InitWindow(screenSize, screenSize, "Raytracer");
  // SetTargetFPS(30);

  RenderTexture2D renderTarget = LoadRenderTexture(camera.resolution, camera.resolution);

  while (!WindowShouldClose()) {
    // auto render_start = std::chrono::high_resolution_clock::now();
    std::vector<int> render = scene.renderScene(camera);
    // auto render_end = std::chrono::high_resolution_clock::now();
    // std::cout << "Render Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(render_end - render_start).count() << "\n";

    cameraControl(camera);
    
    // auto texture_draw_start = std::chrono::high_resolution_clock::now();
    BeginTextureMode(renderTarget);
    ClearBackground(SKYBLUE);
    for (int i = 0; i < render.size(); i++) {
      DrawRectangle(i % camera.resolution, i / camera.resolution, 1, 1, GetColor(render[i]));
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
    // std::cout << "Texture Display Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(texture_display_end - texture_display_start).count() << "\n\n";
  }
  
  return 0;
}