#include "app.h"
#include "MeshBuilder.h"
#include "Shader.h"
#include "Vao.h"
#include "checkerror.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

App::App(float w, int n) {
  _material = PhongMaterial::make();
  auto [m, i] = uniformGrid(w, n);
  _mesh = m;
  _indices = i;

  // fill program input
  _vao = std::make_shared<Vao>();
  _vao->bufferData(0, &_mesh->point(*_mesh->vertices_begin()),
                   _mesh->n_vertices());
  _vao->bufferData(1, &_mesh->normal(*_mesh->vertices_begin()),
                   _mesh->n_vertices());
  _vao->bufferData(2, &_mesh->texcoord2D(*_mesh->vertices_begin()),
                   _mesh->n_vertices());
  _vao->setIndexData(_indices.data(), _indices.size());

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
  _vao->bufferData(0, &_mesh->point(*_mesh->vertices_begin()),
                   _mesh->n_vertices());
  // update vertex normals
  _vao->bufferData(1, &_mesh->normal(*_mesh->vertices_begin()),
                   _mesh->n_vertices());

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
