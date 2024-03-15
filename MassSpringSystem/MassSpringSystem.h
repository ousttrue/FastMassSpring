#pragma once
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <vector>

// Mass-Spring System struct
struct mass_spring_system {
  typedef Eigen::SparseMatrix<float> SparseMatrix;
  typedef Eigen::VectorXf VectorXf;
  typedef std::pair<unsigned int, unsigned int> Edge;
  typedef std::vector<Edge> EdgeList;

  // parameters
  unsigned int n_points;  // number of points
  unsigned int n_springs; // number of springs
  float time_step;        // time step
  EdgeList spring_list;   // spring edge list
  VectorXf rest_lengths;  // spring rest lengths
  VectorXf stiffnesses;   // spring stiffnesses
  VectorXf masses;        // point masses
  VectorXf fext;          // external forces
  float damping_factor;   // damping factor

  mass_spring_system(unsigned int n_points,  // number of points
                     unsigned int n_springs, // number of springs
                     float time_step,        // time step
                     EdgeList spring_list,   // spring edge list
                     VectorXf rest_lengths,  // spring rest lengths
                     VectorXf stiffnesses,   // spring stiffnesses
                     VectorXf masses,        // point masses
                     VectorXf fext,          // external forces
                     float damping_factor    // damping factor
                     )
      : n_points(n_points), n_springs(n_springs), time_step(time_step),
        spring_list(spring_list), rest_lengths(rest_lengths),
        stiffnesses(stiffnesses), masses(masses), fext(fext),
        damping_factor(damping_factor) {}
};
