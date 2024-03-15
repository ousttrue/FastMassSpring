#include "ConstraintGraph.h"
#include "MassSpringSystem.h"
#include <assert.h>

CgNode::CgNode(const std::shared_ptr<mass_spring_system> &system, float *vbuff)
    : system(system), vbuff(vbuff) {}

// point node
CgPointNode::CgPointNode(const std::shared_ptr<mass_spring_system> &system,
                         float *vbuff)
    : CgNode(system, vbuff) {}
bool CgPointNode::accept(CgNodeVisitor &visitor) {
  return visitor.visit(*this);
}

// spring node
CgSpringNode::CgSpringNode(const std::shared_ptr<mass_spring_system> &system,
                           float *vbuff)
    : CgNode(system, vbuff) {}
bool CgSpringNode::accept(CgNodeVisitor &visitor) {
  for (CgNode *child : children) {
    if (!child->accept(visitor))
      return false;
  }
  return visitor.visit(*this);
}
void CgSpringNode::addChild(CgNode *node) { children.push_back(node); }
void CgSpringNode::removeChild(CgNode *node) {
  children.erase(find(children.begin(), children.end(), node));
}

// root node
CgRootNode::CgRootNode(const std::shared_ptr<mass_spring_system> &system,
                       float *vbuff)
    : CgSpringNode(system, vbuff) {}
void CgRootNode::satisfy() { return; }
bool CgRootNode::accept(CgNodeVisitor &visitor) {
  for (CgNode *child : children) {
    if (!child->accept(visitor))
      return false;
  }
  return true;
}

// point fix node
CgPointFixNode::CgPointFixNode(
    const std::shared_ptr<mass_spring_system> &system, float *vbuff)
    : CgPointNode(system, vbuff) {}
bool CgPointFixNode::query(unsigned int i) const {
  return fix_map.find(3 * i) != fix_map.end();
}
void CgPointFixNode::satisfy() {
  for (auto fix : fix_map)
    for (int i = 0; i < 3; i++)
      vbuff[fix.first + i] = fix.second[i];
}
void CgPointFixNode::fixPoint(unsigned int i) {
  assert(i >= 0 && i < system->n_points);
  fix_map[3 * i] = Vector3f(vbuff[3 * i], vbuff[3 * i + 1], vbuff[3 * i + 2]);
}
void CgPointFixNode::releasePoint(unsigned int i) { fix_map.erase(3 * i); }

// spring deformation node
CgSpringDeformationNode::CgSpringDeformationNode(
    const std::shared_ptr<mass_spring_system> &system, float *vbuff, float tauc,
    unsigned int n_iter)
    : CgSpringNode(system, vbuff), tauc(tauc), n_iter(n_iter) {}
void CgSpringDeformationNode::satisfy() {
  for (int k = 0; k < n_iter; k++) {
    for (unsigned int i : items) {
      Edge spring = system->spring_list[i];
      CgQueryFixedPointVisitor visitor;

      Vector3f p12(vbuff[3 * spring.first + 0] - vbuff[3 * spring.second + 0],
                   vbuff[3 * spring.first + 1] - vbuff[3 * spring.second + 1],
                   vbuff[3 * spring.first + 2] - vbuff[3 * spring.second + 2]);

      float len = p12.norm();
      float rlen = system->rest_lengths[i];
      float diff = (len - (1 + tauc) * rlen) / len;
      float rate = (len - rlen) / rlen;

      // check deformation
      if (rate <= tauc)
        continue;

      // check if points are fixed
      float f1, f2;
      f1 = f2 = 0.5f;

      // if first point is fixed
      if (visitor.queryPoint(*this, spring.first)) {
        f1 = 0.0f;
        f2 = 1.0f;
      }

      // if second point is fixed
      if (visitor.queryPoint(*this, spring.second)) {
        f1 = (f1 != 0.0f ? 1.0f : 0.0f);
        f2 = 0.0f;
      }

      for (int j = 0; j < 3; j++) {
        vbuff[3 * spring.first + j] -= p12[j] * f1 * diff;
        vbuff[3 * spring.second + j] += p12[j] * f2 * diff;
      }
    }
  }
}
void CgSpringDeformationNode::addSprings(std::vector<unsigned int> springs) {
  items.insert(springs.begin(), springs.end());
}

// sphere collision node
CgSphereCollisionNode::CgSphereCollisionNode(
    const std::shared_ptr<mass_spring_system> &system, float *vbuff,
    float radius, Vector3f center)
    : CgPointNode(system, vbuff), radius(radius), center(center) {}
bool CgSphereCollisionNode::query(unsigned int i) const { return false; }
void CgSphereCollisionNode::satisfy() {
  for (int i = 0; i < system->n_points; i++) {
    Vector3f p(vbuff[3 * i + 0] - center[0], vbuff[3 * i + 1] - center[1],
               vbuff[3 * i + 2] - center[2]);

    if (p.norm() < radius) {
      p.normalize();
      p = radius * p;
    } else
      continue;

    for (int j = 0; j < 3; j++) {
      vbuff[3 * i + j] = p[j] + center[j];
    }
  }
}

// node visitor
bool CgNodeVisitor::visit(CgPointNode &node) { return true; }
bool CgNodeVisitor::visit(CgSpringNode &node) { return true; }

// query fixed point visitor
bool CgQueryFixedPointVisitor::visit(CgPointNode &node) {
  queryResult = node.query(i);
  return !queryResult;
}
bool CgQueryFixedPointVisitor::queryPoint(CgNode &root, unsigned int i) {
  this->i = i;
  root.accept(*this);
  return queryResult;
}

// satisfy visitor
bool CgSatisfyVisitor::visit(CgPointNode &node) {
  node.satisfy();
  return true;
}
bool CgSatisfyVisitor::visit(CgSpringNode &node) {
  node.satisfy();
  return true;
}
void CgSatisfyVisitor::satisfy(CgNode &root) { root.accept(*this); }
