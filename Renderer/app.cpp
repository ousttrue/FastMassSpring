#include "app.h"
#include "Mesh.h"
#include "Shader.h"
#include "Vao.h"
#include "checkerror.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

App::App(float w, int n) {
  _material = PhongMaterial::make();
  _mesh = Mesh::uniformGrid(w, n);

  // fill program input
  _vao = std::make_shared<Vao>();
  _vao->setPositionData(_mesh->vbuff(), _mesh->vbuffLen());
  _vao->setNormalData(_mesh->nbuff(), _mesh->nbuffLen());
  _vao->setTextureData(_mesh->tbuff(), _mesh->tbuffLen());
  _vao->setIndexData(_mesh->ibuff(), _mesh->ibuffLen());

  // check errors
  checkGlErrors();
}

App::~App() { std::cout << "Tmp::~Tmp" << std::endl; }

void App::drawCloth(const glm::mat4 &proj, const glm::mat4 &view) {
  // update normals
  _mesh->request_face_normals();
  _mesh->update_normals();
  _mesh->release_face_normals();

  // update vertex positions
  _vao->setPositionData(_mesh->vbuff(), _mesh->vbuffLen());

  // update vertex normals
  _vao->setNormalData(_mesh->nbuff(), _mesh->vbuffLen());

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

  auto shader = _material->shader();
  shader->bind();
  {
    shader->setModelView(view);
    shader->setProjection(proj);
    _material->setAlbedo(g_albedo);
    _material->setAmbient(g_ambient);
    _material->setLight(g_light);
    _vao->draw();
    checkGlErrors();
  }
  _material->shader()->unbind();
}
