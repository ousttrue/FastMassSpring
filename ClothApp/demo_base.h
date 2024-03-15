#pragma once
#include <memory>

struct DemoBase {
  std::shared_ptr<struct mass_spring_system> g_system;
  std::shared_ptr<class MassSpringBuilder> _massSpringBuilder;

  DemoBase(const struct SystemParam &param);
  virtual ~DemoBase() {}
};
