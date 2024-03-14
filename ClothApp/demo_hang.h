#pragma once
#include "demo_base.h"

struct DemoHang : DemoBase {
  // Mass Spring System
  struct mass_spring_system *g_system = nullptr;
  class MassSpringSolver *g_solver = nullptr;

  // User Interaction
  std::shared_ptr<class PickMaterial> g_pickShader;

  class UserInteraction *UI = nullptr;

  // Constraint Graph
  class CgRootNode *g_cgRootNode = nullptr;

  DemoHang(const struct SystemParam &param, class Mesh *g_clothMesh,
           const std::shared_ptr<class Vao> &vao);
  ~DemoHang();
  void Animation();
};
