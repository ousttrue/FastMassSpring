#pragma once
#include "demo_base.h"

struct DemoDrop : public DemoBase {
  // Mass Spring System
  class MassSpringSolver *g_solver = nullptr;

  // User Interaction
  class UserInteraction *UI = nullptr;

  // Constraint Graph
  class CgRootNode *g_cgRootNode = nullptr;

  DemoDrop(const struct SystemParam &param, class Mesh *g_clothMesh,
           const std::shared_ptr<class Vao> &vao);
  ~DemoDrop();
  void Animation();
};
