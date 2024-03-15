#pragma once
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <memory>

// Mass-Spring System Solver class
class MassSpringSolver {
private:
  typedef Eigen::Vector3f Vector3f;
  typedef Eigen::VectorXf VectorXf;
  typedef Eigen::SparseMatrix<float> SparseMatrix;
  typedef Eigen::SimplicialLLT<Eigen::SparseMatrix<float>> Cholesky;
  typedef Eigen::Map<Eigen::VectorXf> Map;
  typedef std::pair<unsigned int, unsigned int> Edge;
  typedef Eigen::Triplet<float> Triplet;
  typedef std::vector<Triplet> TripletList;

  // system
  const std::shared_ptr<struct mass_spring_system> &system;
  Cholesky system_matrix;

  // M, L, J matrices
  SparseMatrix M;
  SparseMatrix L;
  SparseMatrix J;

  // state
  Map current_state;          // q(n), current state
  VectorXf prev_state;        // q(n - 1), previous state
  VectorXf spring_directions; // d, spring directions
  VectorXf inertial_term;     // M * y, y = (a + 1) * q(n) - a * q(n - 1)

  // steps
  void globalStep();
  void localStep();

public:
  MassSpringSolver(const std::shared_ptr<mass_spring_system> &system,
                   float *vbuff);

  // solve iterations
  void solve(unsigned int n);
  void timedSolve(unsigned int ms);
};
