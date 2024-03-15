#include "demo_drop.h"
#include "MassSpringSolver.h"
#include "UserInteraction.h"
#include "param.h"

DemoDrop::DemoDrop(const PickCallback &callback, const SystemParam &param,
                   float *vbuff)
    : DemoBase(param) {
  // initialize mass spring solver
  g_solver = std::make_shared<MassSpringSolver>(g_system, vbuff);

  // sphere collision constraint parameters
  const float radius = 0.64f;             // sphere radius | 0.64f
  const Eigen::Vector3f center(0, 0, -1); // sphere center | (0, 0, -1)

  // deformation constraint parameters
  const float tauc = 0.12f;           // critical spring deformation | 0.12f
  const unsigned int deformIter = 15; // number of iterations | 15

  // initialize constraints
  // sphere collision constraint
  CgSphereCollisionNode *sphereCollisionNode =
      new CgSphereCollisionNode(g_system, vbuff, radius, center);

  // spring deformation constraint
  CgSpringDeformationNode *deformationNode =
      new CgSpringDeformationNode(g_system, vbuff, tauc, deformIter);
  deformationNode->addSprings(_massSpringBuilder->getShearIndex());
  deformationNode->addSprings(_massSpringBuilder->getStructIndex());

  // initialize user interaction
  CgPointFixNode *mouseFixer = new CgPointFixNode(g_system, vbuff);
  UI = std::make_shared<GridMeshUI>(callback, mouseFixer, vbuff, param.n);

  // build constraint graph
  g_cgRootNode = std::make_shared<CgRootNode>(g_system, vbuff);

  // first layer
  g_cgRootNode->addChild(deformationNode);
  g_cgRootNode->addChild(sphereCollisionNode);

  // second layer
  deformationNode->addChild(mouseFixer);
}

DemoDrop::~DemoDrop() {}

void DemoDrop::Animation() {
  const int g_iter = 5; // iterations per time step | 10

  // solve two time-steps
  g_solver->solve(g_iter);
  g_solver->solve(g_iter);

  // fix points
  CgSatisfyVisitor visitor;
  visitor.satisfy(*g_cgRootNode);
}
