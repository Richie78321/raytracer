#pragma once
#include <linalg.h>

namespace rt {
  using linalg::aliases::float3;

  struct SceneColor {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;

    SceneColor(unsigned char r, unsigned char g, unsigned char b);
    SceneColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    SceneColor(int hex);

    SceneColor operator*(float intensity) const;
    SceneColor operator*(SceneColor color) const;
    SceneColor operator+(SceneColor other) const;
    int toHex() const;
  };

  const SceneColor SCENE_WHITE { (unsigned char)255, (unsigned char)255, (unsigned char)255, (unsigned char)255 };
  const SceneColor SCENE_BLACK { (unsigned char)0, (unsigned char)0, (unsigned char)0, (unsigned char)255 };
  const SceneColor SKY_BLUE { (unsigned char)94, (unsigned char)149, (unsigned char)241, (unsigned char)255 };

  struct Light {
    float3 position;
    float intensity;
    SceneColor color;

    SceneColor getColorAtPosition(float3 position) const;
  };
}