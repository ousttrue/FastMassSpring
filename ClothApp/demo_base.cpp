#include "demo_base.h"
#include "MassSpringSolver.h"
#include "param.h"

DemoBase::DemoBase(const SystemParam &param)
    : _massSpringBuilder(new MassSpringBuilder) {
  // initialize mass spring system
  _massSpringBuilder->uniformGrid(param.n, param.h, param.r, param.k, param.m,
                                  param.a, param.g);
  g_system = _massSpringBuilder->getResult();
}
