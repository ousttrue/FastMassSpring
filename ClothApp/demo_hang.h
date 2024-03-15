#pragma once
#include "demo_base.h"
#include "pickcallback.h"

struct DemoHang : DemoBase {
  // Mass Spring System
  class MassSpringSolver *g_solver = nullptr;

  // User Interaction
  class UserInteraction *UI = nullptr;

  // Constraint Graph
  class CgRootNode *g_cgRootNode = nullptr;

  DemoHang(const PickCallback &callback, const struct SystemParam &param,
           float *vbuff);
  ~DemoHang();
  void Animation();
};
