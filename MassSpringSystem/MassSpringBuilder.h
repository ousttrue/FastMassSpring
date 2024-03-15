#pragma once
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <assert.h>
#include <memory>
#include <vector>

// Mass-Spring System Builder Class
class MassSpringBuilder {
private:
  typedef Eigen::Vector3f Vector3f;
  typedef Eigen::VectorXf VectorXf;
  typedef std::pair<unsigned int, unsigned int> Edge;
  typedef std::vector<Edge> EdgeList;
  typedef Eigen::Triplet<float> Triplet;
  typedef std::vector<Triplet> TripletList;
  typedef std::vector<unsigned int> IndexList;

  IndexList structI, shearI, bendI;
  std::shared_ptr<struct mass_spring_system> result;

public:
  void uniformGrid(unsigned int n,       // grid width
                   float time_step,      // time step
                   float rest_length,    // spring rest length (non-diagonal)
                   float stiffness,      // spring stiffness
                   float mass,           // node mass
                   float damping_factor, // damping factor
                   float gravity         // gravitationl force (-z axis)
  );
  // indices
  // structural springs
  MassSpringBuilder::IndexList getStructIndex() { return structI; }
  // shearing springs
  MassSpringBuilder::IndexList getShearIndex() { return shearI; }
  // bending springs
  MassSpringBuilder::IndexList getBendIndex() { return bendI; }

  std::shared_ptr<mass_spring_system> getResult() { return result; }
};
