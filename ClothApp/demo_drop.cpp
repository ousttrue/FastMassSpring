#include "demo_drop.h"
#include "MassSpringSolver.h"
#include "Mesh.h"
#include "Shader.h"
#include "UserInteraction.h"
#include "param.h"

DemoDrop::DemoDrop(const SystemParam &param, const std::shared_ptr<Mesh> &mesh,
                   const std::shared_ptr<class Vao> &vao)
    : DemoBase(param) {
  // initialize mass spring solver
  g_solver = std::make_shared<MassSpringSolver>(g_system, mesh->vbuff());

  // sphere collision constraint parameters
  const float radius = 0.64f;             // sphere radius | 0.64f
  const Eigen::Vector3f center(0, 0, -1); // sphere center | (0, 0, -1)

  // deformation constraint parameters
  const float tauc = 0.12f;           // critical spring deformation | 0.12f
  const unsigned int deformIter = 15; // number of iterations | 15

  // initialize constraints
  // sphere collision constraint
  CgSphereCollisionNode *sphereCollisionNode =
      new CgSphereCollisionNode(g_system, mesh->vbuff(), radius, center);

  // spring deformation constraint
  CgSpringDeformationNode *deformationNode =
      new CgSpringDeformationNode(g_system, mesh->vbuff(), tauc, deformIter);
  deformationNode->addSprings(_massSpringBuilder->getShearIndex());
  deformationNode->addSprings(_massSpringBuilder->getStructIndex());

  // initialize user interaction
  auto shader = g_pickShader->shader();
  g_pickShader->setTessFact(param.n);
  CgPointFixNode *mouseFixer = new CgPointFixNode(g_system, mesh->vbuff());
  UI = std::make_shared<GridMeshUI>(g_pickShader->shader(), vao, mouseFixer,
                                    mesh->vbuff(), param.n);

  // build constraint graph
  g_cgRootNode = std::make_shared<CgRootNode>(g_system, mesh->vbuff());

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
