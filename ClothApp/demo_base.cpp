#include "demo_base.h"
#include "Shader.h"
#include "param.h"

DemoBase::DemoBase(const SystemParam &param) {
  auto ibasic = readall("./ClothApp/shaders/basic.vshader");
  assert(ibasic.size());
  GLShader basic_vert(GL_VERTEX_SHADER);
  basic_vert.compile(ibasic);

  auto ifrag = readall("./ClothApp/shaders/pick.fshader");
  assert(ifrag.size());
  GLShader pick_frag(GL_FRAGMENT_SHADER);
  pick_frag.compile(ifrag);

  g_pickShader = new PickShader;
  g_pickShader->link(basic_vert, pick_frag);

  // initialize mass spring system
  massSpringBuilder.uniformGrid(param.n, param.h, param.r, param.k, param.m,
                                param.a, param.g);
  g_system = massSpringBuilder.getResult();
}

DemoBase::~DemoBase() {
  delete g_system;
  delete g_pickShader;
}
