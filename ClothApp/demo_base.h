#pragma once
#include "MassSpringSolver.h"

struct DemoBase {
  class PickShader *g_pickShader = nullptr; // linked pick shader
  struct mass_spring_system *g_system = nullptr;
  MassSpringBuilder massSpringBuilder;

  DemoBase(const struct SystemParam &param);
  virtual ~DemoBase();
};
