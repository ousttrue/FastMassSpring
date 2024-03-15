#pragma once
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// Constraint Graph
class CgNodeVisitor; // Constraint graph node visitor

// Constraint graph node
class CgNode {
protected:
  std::shared_ptr<struct mass_spring_system> system;
  float *vbuff;

public:
  CgNode(const std::shared_ptr<mass_spring_system> &system, float *vbuff);

  virtual void satisfy() = 0;                      // satisfy constraint
  virtual bool accept(CgNodeVisitor &visitor) = 0; // accept visitor
};

// point constraint node
class CgPointNode : public CgNode {
public:
  CgPointNode(const std::shared_ptr<mass_spring_system> &system, float *vbuff);
  virtual bool
  query(unsigned int i) const = 0; // check if point with index i is constrained
  virtual bool accept(CgNodeVisitor &visitor);
};

// spring constraint node
class CgSpringNode : public CgNode {
protected:
  typedef std::vector<CgNode *> NodeList;
  NodeList children;

public:
  CgSpringNode(const std::shared_ptr<mass_spring_system> &system, float *vbuff);

  virtual bool accept(CgNodeVisitor &visitor);
  void addChild(CgNode *node);
  void removeChild(CgNode *node);
};

// root node
class CgRootNode : public CgSpringNode {
public:
  CgRootNode(const std::shared_ptr<mass_spring_system> &system, float *vbuff);

  virtual void satisfy();
  virtual bool accept(CgNodeVisitor &visitor);
};

class CgPointFixNode : public CgPointNode {
protected:
  typedef Eigen::Vector3f Vector3f;
  std::unordered_map<unsigned int, Vector3f> fix_map;

public:
  CgPointFixNode(const std::shared_ptr<mass_spring_system> &system,
                 float *vbuff);
  virtual void satisfy();

  virtual bool query(unsigned int i) const;
  virtual void fixPoint(unsigned int i); // add point at index i to list
  virtual void
  releasePoint(unsigned int i); // remove point at index i from list
};

// spring node
class CgSpringDeformationNode : public CgSpringNode {
private:
  typedef std::pair<unsigned int, unsigned int> Edge;
  typedef Eigen::Vector3f Vector3f;
  std::unordered_set<unsigned int> items;
  float tauc;          // critical deformation rate
  unsigned int n_iter; // number of iterations

public:
  CgSpringDeformationNode(const std::shared_ptr<mass_spring_system> &system,
                          float *vbuff, float tauc, unsigned int n_iter);
  virtual void satisfy();

  void addSprings(std::vector<unsigned int> springs);
};

// sphere collision node
class CgSphereCollisionNode : public CgPointNode {
private:
  typedef Eigen::Vector3f Vector3f;

  float radius;
  Vector3f center;

public:
  CgSphereCollisionNode(const std::shared_ptr<mass_spring_system> &system,
                        float *vbuff, float radius, Vector3f center);
  virtual bool query(unsigned int i) const;
  virtual void satisfy();
};

// node visitor
class CgNodeVisitor {
public:
  virtual bool visit(CgPointNode &node);
  virtual bool visit(CgSpringNode &node);
};

// fixed point query visitor
class CgQueryFixedPointVisitor : public CgNodeVisitor {
private:
  unsigned int i;
  bool queryResult;

public:
  virtual bool visit(CgPointNode &node);

  bool queryPoint(CgNode &root, unsigned int i);
};

// satisfy visitor
class CgSatisfyVisitor : public CgNodeVisitor {
public:
  virtual bool visit(CgPointNode &node);
  virtual bool visit(CgSpringNode &node);

  void satisfy(CgNode &root);
};
