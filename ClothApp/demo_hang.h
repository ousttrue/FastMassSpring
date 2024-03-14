#pragma once
#include "demo_base.h"

struct DemoHang : DemoBase {
  // Mass Spring System
  struct mass_spring_system *g_system = nullptr;
  class MassSpringSolver *g_solver = nullptr;

  // User Interaction
  class PickShader *g_pickShader = nullptr; // linked pick shader
  class Renderer *g_pickRenderer = nullptr;

  class UserInteraction *UI = nullptr;

  // Constraint Graph
  class CgRootNode *g_cgRootNode = nullptr;

  DemoHang(const struct SystemParam &param, class Mesh *g_clothMesh,
           class ProgramInput *g_render_target);
  ~DemoHang();
  void Animation();
};
