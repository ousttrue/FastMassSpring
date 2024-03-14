#pragma once
#include "demo_base.h"

struct DemoDrop : public DemoBase {
  // Mass Spring System
  struct mass_spring_system *g_system = nullptr;
  class MassSpringSolver *g_solver = nullptr;

  // User Interaction
  class Renderer *g_pickRenderer = nullptr;

  class UserInteraction *UI = nullptr;

  // Constraint Graph
  class CgRootNode *g_cgRootNode = nullptr;

  DemoDrop(const struct SystemParam &param, class Mesh *g_clothMesh,
           class ProgramInput *g_render_target);
  ~DemoDrop();
  void Animation();
};
