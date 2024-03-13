#include "app.h"
#include "param.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "MassSpringSolver.h"
#include "Mesh.h"
#include "Renderer.h"
#include "Shader.h"
#include "UserInteraction.h"

static void checkGlErrors() {
  const GLenum errCode = glGetError();
  if (errCode != GL_NO_ERROR) {
    std::string error("GL Error: ");
    error += reinterpret_cast<const char *>(gluErrorString(errCode));
    std::cerr << error << std::endl;
    throw std::runtime_error(error);
  }
}

Demo::Demo(const SystemParam &param) {

  auto ibasic = std::ifstream("./ClothApp/shaders/basic.vshader");
  assert(ibasic);
  GLShader basic_vert(GL_VERTEX_SHADER);
  basic_vert.compile(ibasic);

  auto iphong = std::ifstream("./ClothApp/shaders/phong.fshader");
  assert(iphong);
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

  // //
  // //
  // //
  // // initialize mass spring system
  // MassSpringBuilder massSpringBuilder;
  // massSpringBuilder.uniformGrid(param.n, param.h, param.r, param.k, param.m,
  //                               param.a, param.g);
  // g_system = massSpringBuilder.getResult();
  //
  // // initialize mass spring solver
  // g_solver = new MassSpringSolver(g_system, g_clothMesh->vbuff());
  //
  // // sphere collision constraint parameters
  // const float radius = 0.64f;             // sphere radius | 0.64f
  // const Eigen::Vector3f center(0, 0, -1); // sphere center | (0, 0, -1)
  //
  // // deformation constraint parameters
  // const float tauc = 0.12f;           // critical spring deformation | 0.12f
  // const unsigned int deformIter = 15; // number of iterations | 15
  //
  // // initialize constraints
  // // sphere collision constraint
  // CgSphereCollisionNode *sphereCollisionNode =
  //     new CgSphereCollisionNode(g_system, g_clothMesh->vbuff(), radius, center);
  //
  // // spring deformation constraint
  // CgSpringDeformationNode *deformationNode = new CgSpringDeformationNode(
  //     g_system, g_clothMesh->vbuff(), tauc, deformIter);
  // deformationNode->addSprings(massSpringBuilder.getShearIndex());
  // deformationNode->addSprings(massSpringBuilder.getStructIndex());
  //
  // // initialize user interaction
  // g_pickRenderer = new Renderer();
  // g_pickRenderer->setProgram(g_pickShader);
  // g_pickRenderer->setProgramInput(g_render_target);
  // g_pickRenderer->setElementCount(g_clothMesh->ibuffLen());
  // g_pickShader->setTessFact(param.n);
  // CgPointFixNode *mouseFixer =
  //     new CgPointFixNode(g_system, g_clothMesh->vbuff());
  // UI =
  //     new GridMeshUI(g_pickRenderer, mouseFixer, g_clothMesh->vbuff(), param.n);
  //
  // // build constraint graph
  // g_cgRootNode = new CgRootNode(g_system, g_clothMesh->vbuff());
  //
  // // first layer
  // g_cgRootNode->addChild(deformationNode);
  // g_cgRootNode->addChild(sphereCollisionNode);
  //
  // // second layer
  // deformationNode->addChild(mouseFixer);

  //
  //
  //
  glClearColor(0.25f, 0.25f, 0.25f, 0);
  glClearDepth(1.);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glReadBuffer(GL_BACK);
  glEnable(GL_FRAMEBUFFER_SRGB);

  checkGlErrors();
}

Demo::~Demo() {
  std::cout << "Tmp::~Tmp" << std::endl;

  // delete mesh
  delete g_clothMesh;

  // delete render target
  delete g_render_target;

  // delete constraint graph
  // TODO
}

// C L O T H
// ///////////////////////////////////////////////////////////////////////
void Demo::drawCloth(const glm::mat4 &proj, const glm::mat4 &view) {
  // update normals
  g_clothMesh->request_face_normals();
  g_clothMesh->update_normals();
  g_clothMesh->release_face_normals();

  // update target
  // update vertex positions
  g_render_target->setPositionData(g_clothMesh->vbuff(),
                                   g_clothMesh->vbuffLen());

  // update vertex normals
  g_render_target->setNormalData(g_clothMesh->nbuff(), g_clothMesh->vbuffLen());

  //
  // render
  //
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
