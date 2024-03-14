#include "app.h"
#include "Mesh.h"
#include "Renderer.h"
#include "Shader.h"
#include "param.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <stdexcept>
#include <string>

static void checkGlErrors() {
  const GLenum errCode = glGetError();
  if (errCode != GL_NO_ERROR) {
    std::string error("GL Error: ");
    error += reinterpret_cast<const char *>(gluErrorString(errCode));
    std::cerr << error << std::endl;
    throw std::runtime_error(error);
  }
}

App::App(const SystemParam &param) {

  auto ibasic = readall("./ClothApp/shaders/basic.vshader");
  assert(ibasic.size());
  GLShader basic_vert(GL_VERTEX_SHADER);
  basic_vert.compile(ibasic);

  auto iphong = readall("./ClothApp/shaders/phong.fshader");
  assert(iphong.size());
  GLShader phong_frag(GL_FRAGMENT_SHADER);
  phong_frag.compile(iphong);

  g_phongShader = new PhongShader;
  g_phongShader->link(basic_vert, phong_frag);

  checkGlErrors();

  // generate mesh
  MeshBuilder meshBuilder;
  meshBuilder.uniformGrid(param.w, param.n);
  g_clothMesh = meshBuilder.getResult();

  // fill program input
  g_render_target = new ProgramInput;
  g_render_target->setPositionData(g_clothMesh->vbuff(),
                                   g_clothMesh->vbuffLen());
  g_render_target->setNormalData(g_clothMesh->nbuff(), g_clothMesh->nbuffLen());
  g_render_target->setTextureData(g_clothMesh->tbuff(),
                                  g_clothMesh->tbuffLen());
  g_render_target->setIndexData(g_clothMesh->ibuff(), g_clothMesh->ibuffLen());

  // check errors
  checkGlErrors();
}

App::~App() {
  std::cout << "Tmp::~Tmp" << std::endl;

  // delete mesh
  delete g_clothMesh;

  // delete render target
  delete g_render_target;

  // delete constraint graph
  // TODO
}

void App::drawCloth(const glm::mat4 &proj, const glm::mat4 &view) {
  // update normals
  g_clothMesh->request_face_normals();
  g_clothMesh->update_normals();
  g_clothMesh->release_face_normals();

  // update vertex positions
  g_render_target->setPositionData(g_clothMesh->vbuff(),
                                   g_clothMesh->vbuffLen());

  // update vertex normals
  g_render_target->setNormalData(g_clothMesh->nbuff(), g_clothMesh->vbuffLen());

  //
  // render
  //
  glClearColor(0.25f, 0.25f, 0.25f, 0);
  glClearDepth(1.);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glReadBuffer(GL_BACK);
  glEnable(GL_FRAMEBUFFER_SRGB);

  Renderer renderer;
  renderer.setProgram(g_phongShader);
  renderer.setModelview(view);
  renderer.setProjection(proj);
  g_phongShader->setAlbedo(g_albedo);
  g_phongShader->setAmbient(g_ambient);
  g_phongShader->setLight(g_light);
  renderer.setProgramInput(g_render_target);
  renderer.setElementCount(g_clothMesh->ibuffLen());
  renderer.draw();

  checkGlErrors();
}
