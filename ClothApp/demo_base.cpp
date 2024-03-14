#include "demo_base.h"
#include "Shader.h"

DemoBase::DemoBase() {
  auto ibasic = std::ifstream("./ClothApp/shaders/basic.vshader");
  assert(ibasic);
  GLShader basic_vert(GL_VERTEX_SHADER);
  basic_vert.compile(ibasic);

  auto ifrag = std::ifstream("./ClothApp/shaders/pick.fshader");
  assert(ifrag);
  GLShader pick_frag(GL_FRAGMENT_SHADER);
  pick_frag.compile(ifrag);

  g_pickShader = new PickShader;
  g_pickShader->link(basic_vert, pick_frag);
}

DemoBase::~DemoBase() { delete g_pickShader; }
