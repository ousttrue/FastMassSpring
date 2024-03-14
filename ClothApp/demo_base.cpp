#include "demo_base.h"
#include "Shader.h"
#include "param.h"

DemoBase::DemoBase(const SystemParam &param) {

  g_pickShader = PickMaterial::make();

  // initialize mass spring system
  massSpringBuilder.uniformGrid(param.n, param.h, param.r, param.k, param.m,
                                param.a, param.g);
  g_system = massSpringBuilder.getResult();
}

DemoBase::~DemoBase() {
  delete g_system;
}
