#pragma once
#include "MassSpringSolver.h"
#include <memory>

struct DemoBase {
  std::shared_ptr<class PickMaterial> g_pickShader;
  struct mass_spring_system *g_system = nullptr;
  MassSpringBuilder massSpringBuilder;

  DemoBase(const struct SystemParam &param);
  virtual ~DemoBase();
};
