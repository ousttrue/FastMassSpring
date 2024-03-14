#include "app.h"
#include "Mesh.h"
#include "Shader.h"
#include "Vao.h"
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

  g_phongShader = PhongMaterial::make();

  // generate mesh
  MeshBuilder meshBuilder;
  meshBuilder.uniformGrid(param.w, param.n);
  g_clothMesh = meshBuilder.getResult();

  // fill program input
  _vao = std::make_shared<Vao>();
  _vao->setPositionData(g_clothMesh->vbuff(), g_clothMesh->vbuffLen());
  _vao->setNormalData(g_clothMesh->nbuff(), g_clothMesh->nbuffLen());
  _vao->setTextureData(g_clothMesh->tbuff(), g_clothMesh->tbuffLen());
  _vao->setIndexData(g_clothMesh->ibuff(), g_clothMesh->ibuffLen());

  // check errors
  checkGlErrors();
}

App::~App() {
  std::cout << "Tmp::~Tmp" << std::endl;

  // delete mesh
  delete g_clothMesh;

  // delete render target

  // delete constraint graph
  // TODO
}

void App::drawCloth(const glm::mat4 &proj, const glm::mat4 &view) {
  // update normals
  g_clothMesh->request_face_normals();
  g_clothMesh->update_normals();
  g_clothMesh->release_face_normals();

  // update vertex positions
  _vao->setPositionData(g_clothMesh->vbuff(), g_clothMesh->vbuffLen());

  // update vertex normals
  _vao->setNormalData(g_clothMesh->nbuff(), g_clothMesh->vbuffLen());

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

  auto shader = g_phongShader->shader();
  shader->bind();
  {
    shader->setModelView(view);
    shader->setProjection(proj);
    g_phongShader->setAlbedo(g_albedo);
    g_phongShader->setAmbient(g_ambient);
    g_phongShader->setLight(g_light);
    _vao->draw();
    checkGlErrors();
  }
  g_phongShader->shader()->unbind();
}
