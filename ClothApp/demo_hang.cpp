#include "demo_hang.h"
#include "MassSpringSolver.h"
#include "Mesh.h"
#include "Shader.h"
#include "UserInteraction.h"
#include "param.h"

DemoHang::DemoHang(const SystemParam &param, Mesh *g_clothMesh,
                   const std::shared_ptr<Vao> &vao)
    : DemoBase(param) {
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
  g_pickShader->setTessFact(param.n);
  CgPointFixNode *mouseFixer =
      new CgPointFixNode(g_system, g_clothMesh->vbuff());
  UI = new GridMeshUI(g_pickShader->shader(), vao, mouseFixer,
                      g_clothMesh->vbuff(), param.n);

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
  delete UI;

  // delete mass-spring system
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
