#include "demo_hang.h"
#include "MassSpringSolver.h"
#include "Mesh.h"
#include "Renderer.h"
#include "UserInteraction.h"
#include "param.h"

DemoHang::DemoHang(const SystemParam &param, Mesh *g_clothMesh,
                   class ProgramInput *g_render_target) {
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
  cornerFixer->fixPoint(param.n - 1);

  // initialize user interaction
  g_pickRenderer = new Renderer();
  g_pickRenderer->setProgram(g_pickShader);
  g_pickRenderer->setProgramInput(g_render_target);
  g_pickRenderer->setElementCount(g_clothMesh->ibuffLen());
  g_pickShader->setTessFact(param.n);
  CgPointFixNode *mouseFixer =
      new CgPointFixNode(g_system, g_clothMesh->vbuff());
  UI =
      new GridMeshUI(g_pickRenderer, mouseFixer, g_clothMesh->vbuff(), param.n);

  // build constraint graph
  g_cgRootNode = new CgRootNode(g_system, g_clothMesh->vbuff());

  // first layer
  g_cgRootNode->addChild(deformationNode);

  // second layer
  deformationNode->addChild(cornerFixer);
  deformationNode->addChild(mouseFixer);
}

DemoHang::~DemoHang() {
  // delete UI
  delete g_pickRenderer;
  delete UI;

  // delete mass-spring system
  delete g_system;
  delete g_solver;
}

void DemoHang::Animation() {
  const int g_iter = 5; // iterations per time step | 10

  // solve two time-steps
  g_solver->solve(g_iter);
  g_solver->solve(g_iter);

  // fix points
  CgSatisfyVisitor visitor;
  visitor.satisfy(*g_cgRootNode);
}
