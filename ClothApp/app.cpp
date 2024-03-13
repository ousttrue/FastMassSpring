#include "app.h"
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

// G L O B A L S
// ///////////////////////////////////////////////////////////////////

// User Interaction
UserInteraction *UI = nullptr;
static Renderer *g_pickRenderer = nullptr;

// Shader Handles
static PhongShader *g_phongShader = nullptr; // linked phong shader
static PickShader *g_pickShader = nullptr;   // linked pick shader

// Shader parameters
static const glm::vec3 g_albedo(0.0f, 0.3f, 0.7f);
static const glm::vec3 g_ambient(0.01f, 0.01f, 0.01f);
static const glm::vec3 g_light(1.0f, 1.0f, -1.0f);

// Mesh
static Mesh *g_clothMesh = nullptr; // halfedge data structure

// Render Target
static ProgramInput *g_render_target =
    nullptr; // vertex, normal, texutre, index

// Mass Spring System
static mass_spring_system *g_system = nullptr;
static MassSpringSolver *g_solver = nullptr;

// Constraint Graph
static CgRootNode *g_cgRootNode = nullptr;

// F U N C T I O N S
// ////////////////////////////////////////////////////////////// state

// demos
using SystemFunc = void (*)(const SystemParam &param);
static void
demo_hang(const SystemParam &param); // curtain hanging from top corners
static void demo_drop(const SystemParam &param); // curtain dropping on sphere

static SystemFunc g_demo = demo_drop;

// error checks
void checkGlErrors();

// S T A T E  I N I T I A L I Z A T O N
// /////////////////////////////////////////////

void initGLState() {
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

void initShaders() {
  GLShader basic_vert(GL_VERTEX_SHADER);
  GLShader phong_frag(GL_FRAGMENT_SHADER);
  GLShader pick_frag(GL_FRAGMENT_SHADER);

  auto ibasic = std::ifstream("./ClothApp/shaders/basic.vshader");
  assert(ibasic);
  auto iphong = std::ifstream("./ClothApp/shaders/phong.fshader");
  assert(iphong);
  auto ifrag = std::ifstream("./ClothApp/shaders/pick.fshader");
  assert(ifrag);

  basic_vert.compile(ibasic);
  phong_frag.compile(iphong);
  pick_frag.compile(ifrag);

  g_phongShader = new PhongShader;
  g_pickShader = new PickShader;
  g_phongShader->link(basic_vert, phong_frag);
  g_pickShader->link(basic_vert, pick_frag);

  checkGlErrors();
}

void initCloth(const SystemParam &param) {
  // short hand
  const int n = param.n;
  const float w = param.w;

  // generate mesh
  MeshBuilder meshBuilder;
  meshBuilder.uniformGrid(w, n);
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

  // build demo system
  g_demo(param);
}

static void demo_hang(const SystemParam &param) {
  // short hand
  const int n = param.n;

  // initialize mass spring system
  MassSpringBuilder massSpringBuilder;
  massSpringBuilder.uniformGrid(param.n, param.h, param.r, param.k, param.m,
                                param.a, param.g);
  g_system = massSpringBuilder.getResult();

  // initialize mass spring solver
  g_solver = new MassSpringSolver(g_system, g_clothMesh->vbuff());

  // deformation constraint parameters
  const float tauc = 0.4f;            // critical spring deformation | 0.4f
  const unsigned int deformIter = 15; // number of iterations | 15

  // initialize constraints
  // spring deformation constraint
  CgSpringDeformationNode *deformationNode = new CgSpringDeformationNode(
      g_system, g_clothMesh->vbuff(), tauc, deformIter);
  deformationNode->addSprings(massSpringBuilder.getShearIndex());
  deformationNode->addSprings(massSpringBuilder.getStructIndex());

  // fix top corners
  CgPointFixNode *cornerFixer =
      new CgPointFixNode(g_system, g_clothMesh->vbuff());
  cornerFixer->fixPoint(0);
  cornerFixer->fixPoint(n - 1);

  // initialize user interaction
  g_pickRenderer = new Renderer();
  g_pickRenderer->setProgram(g_pickShader);
  g_pickRenderer->setProgramInput(g_render_target);
  g_pickRenderer->setElementCount(g_clothMesh->ibuffLen());
  g_pickShader->setTessFact(param.n);
  CgPointFixNode *mouseFixer =
      new CgPointFixNode(g_system, g_clothMesh->vbuff());
  UI = new GridMeshUI(g_pickRenderer, mouseFixer, g_clothMesh->vbuff(), n);

  // build constraint graph
  g_cgRootNode = new CgRootNode(g_system, g_clothMesh->vbuff());

  // first layer
  g_cgRootNode->addChild(deformationNode);

  // second layer
  deformationNode->addChild(cornerFixer);
  deformationNode->addChild(mouseFixer);
}

static void demo_drop(const SystemParam &param) {
  // short hand
  const int n = param.n;

  // initialize mass spring system
  MassSpringBuilder massSpringBuilder;
  massSpringBuilder.uniformGrid(param.n, param.h, param.r, param.k, param.m,
                                param.a, param.g);
  g_system = massSpringBuilder.getResult();

  // initialize mass spring solver
  g_solver = new MassSpringSolver(g_system, g_clothMesh->vbuff());

  // sphere collision constraint parameters
  const float radius = 0.64f;             // sphere radius | 0.64f
  const Eigen::Vector3f center(0, 0, -1); // sphere center | (0, 0, -1)

  // deformation constraint parameters
  const float tauc = 0.12f;           // critical spring deformation | 0.12f
  const unsigned int deformIter = 15; // number of iterations | 15

  // initialize constraints
  // sphere collision constraint
  CgSphereCollisionNode *sphereCollisionNode =
      new CgSphereCollisionNode(g_system, g_clothMesh->vbuff(), radius, center);

  // spring deformation constraint
  CgSpringDeformationNode *deformationNode = new CgSpringDeformationNode(
      g_system, g_clothMesh->vbuff(), tauc, deformIter);
  deformationNode->addSprings(massSpringBuilder.getShearIndex());
  deformationNode->addSprings(massSpringBuilder.getStructIndex());

  // initialize user interaction
  g_pickRenderer = new Renderer();
  g_pickRenderer->setProgram(g_pickShader);
  g_pickRenderer->setProgramInput(g_render_target);
  g_pickRenderer->setElementCount(g_clothMesh->ibuffLen());
  g_pickShader->setTessFact(param.n);
  CgPointFixNode *mouseFixer =
      new CgPointFixNode(g_system, g_clothMesh->vbuff());
  UI = new GridMeshUI(g_pickRenderer, mouseFixer, g_clothMesh->vbuff(), n);

  // build constraint graph
  g_cgRootNode = new CgRootNode(g_system, g_clothMesh->vbuff());

  // first layer
  g_cgRootNode->addChild(deformationNode);
  g_cgRootNode->addChild(sphereCollisionNode);

  // second layer
  deformationNode->addChild(mouseFixer);
}

// C L O T H
// ///////////////////////////////////////////////////////////////////////
void drawCloth(const glm::mat4 &proj, const glm::mat4 &view) {
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
}

static const int g_iter = 5; // iterations per time step | 10

void animateCloth(int value) {

  // solve two time-steps
  g_solver->solve(g_iter);
  g_solver->solve(g_iter);

  // fix points
  CgSatisfyVisitor visitor;
  visitor.satisfy(*g_cgRootNode);

  // update normals
  g_clothMesh->request_face_normals();
  g_clothMesh->update_normals();
  g_clothMesh->release_face_normals();

  // update target
  updateRenderTarget();
}

// S C E N E  U P D A T E
// ///////////////////////////////////////////////////////////
static void updateRenderTarget() {
  // update vertex positions
  g_render_target->setPositionData(g_clothMesh->vbuff(),
                                   g_clothMesh->vbuffLen());

  // update vertex normals
  g_render_target->setNormalData(g_clothMesh->nbuff(), g_clothMesh->vbuffLen());
}

// C L E A N  U P
// //////////////////////////////////////////////////////////////////
void cleanUp() {
  // delete mesh
  delete g_clothMesh;

  // delete UI
  delete g_pickRenderer;
  delete UI;

  // delete render target
  delete g_render_target;

  // delete mass-spring system
  delete g_system;
  delete g_solver;

  // delete constraint graph
  // TODO
}

// E R R O R S
// /////////////////////////////////////////////////////////////////////
void checkGlErrors() {
  const GLenum errCode = glGetError();

  if (errCode != GL_NO_ERROR) {
    std::string error("GL Error: ");
    error += reinterpret_cast<const char *>(gluErrorString(errCode));
    std::cerr << error << std::endl;
    throw std::runtime_error(error);
  }
}
