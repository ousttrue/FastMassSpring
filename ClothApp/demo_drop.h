#pragma once
#include "demo_base.h"
#include <memory>

struct DemoDrop : public DemoBase {
  // Mass Spring System
  std::shared_ptr<class MassSpringSolver> g_solver;
  // User Interaction
  std::shared_ptr<class UserInteraction> UI;
  // Constraint Graph
  std::shared_ptr<class CgRootNode> g_cgRootNode;

  DemoDrop(const struct SystemParam &param,
           const std::shared_ptr<class Mesh> &mesh,
           const std::shared_ptr<class Vao> &vao);
  ~DemoDrop();
  void Animation();
};
