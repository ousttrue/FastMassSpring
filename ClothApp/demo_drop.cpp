#include "demo_drop.h"
#include "MassSpringSolver.h"
#include "Mesh.h"
#include "Renderer.h"
#include "UserInteraction.h"
#include "param.h"

DemoDrop::DemoDrop(const SystemParam &param, Mesh *g_clothMesh,
                   class ProgramInput *g_render_target)
    : DemoBase(param) {
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
  UI =
      new GridMeshUI(g_pickRenderer, mouseFixer, g_clothMesh->vbuff(), param.n);

  // build constraint graph
  g_cgRootNode = new CgRootNode(g_system, g_clothMesh->vbuff());

  // first layer
  g_cgRootNode->addChild(deformationNode);
  g_cgRootNode->addChild(sphereCollisionNode);

  // second layer
  deformationNode->addChild(mouseFixer);
}

DemoDrop::~DemoDrop() {
  // delete UI
  delete g_pickRenderer;
  delete UI;

  // delete mass-spring system
  delete g_solver;
}

void DemoDrop::Animation() {
  const int g_iter = 5; // iterations per time step | 10

  // solve two time-steps
  g_solver->solve(g_iter);
  g_solver->solve(g_iter);

  // fix points
  CgSatisfyVisitor visitor;
  visitor.satisfy(*g_cgRootNode);
}
