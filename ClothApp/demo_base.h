#pragma once

struct DemoBase {
  class PickShader *g_pickShader = nullptr; // linked pick shader

  DemoBase();
  virtual ~DemoBase();
};
