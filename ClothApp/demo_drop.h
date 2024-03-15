#pragma once
#include "demo_base.h"
#include "pickcallback.h"
#include <memory>

struct DemoDrop : public DemoBase {
  // Mass Spring System
  std::shared_ptr<class MassSpringSolver> g_solver;
  // User Interaction
  std::shared_ptr<class UserInteraction> UI;
  // Constraint Graph
  std::shared_ptr<class CgRootNode> g_cgRootNode;

  DemoDrop(const PickCallback &callback, const struct SystemParam &param,
           float *vbuff);
  ~DemoDrop();
  void Animation();
};
