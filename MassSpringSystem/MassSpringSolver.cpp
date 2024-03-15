#include "MassSpringSolver.h"
#include "MassSpringSystem.h"
#include <iostream>

MassSpringSolver::MassSpringSolver(
    const std::shared_ptr<mass_spring_system> &system, float *vbuff)
    : system(system), current_state(vbuff, system->n_points * 3),
      prev_state(current_state), spring_directions(system->n_springs * 3) {

  float h2 = system->time_step * system->time_step; // shorthand

  // compute M, L, J
  TripletList LTriplets, JTriplets;
  // L
  L.resize(3 * system->n_points, 3 * system->n_points);
  unsigned int k = 0; // spring counter
  for (Edge &i : system->spring_list) {
    for (int j = 0; j < 3; j++) {
      LTriplets.push_back(Triplet(3 * i.first + j, 3 * i.first + j,
                                  1 * system->stiffnesses[k]));
      LTriplets.push_back(Triplet(3 * i.first + j, 3 * i.second + j,
                                  -1 * system->stiffnesses[k]));
      LTriplets.push_back(Triplet(3 * i.second + j, 3 * i.first + j,
                                  -1 * system->stiffnesses[k]));
      LTriplets.push_back(Triplet(3 * i.second + j, 3 * i.second + j,
                                  1 * system->stiffnesses[k]));
    }
    k++;
  }
  L.setFromTriplets(LTriplets.begin(), LTriplets.end());

  // J
  J.resize(3 * system->n_points, 3 * system->n_springs);
  k = 0; // spring counter
  for (Edge &i : system->spring_list) {
    for (unsigned int j = 0; j < 3; j++) {
      JTriplets.push_back(
          Triplet(3 * i.first + j, 3 * k + j, 1 * system->stiffnesses[k]));
      JTriplets.push_back(
          Triplet(3 * i.second + j, 3 * k + j, -1 * system->stiffnesses[k]));
    }
    k++;
  }
  J.setFromTriplets(JTriplets.begin(), JTriplets.end());

  // M
  TripletList MTriplets;
  M.resize(3 * system->n_points, 3 * system->n_points);
  for (unsigned int i = 0; i < system->n_points; i++) {
    for (int j = 0; j < 3; j++) {
      MTriplets.push_back(Triplet(3 * i + j, 3 * i + j, system->masses[i]));
    }
  }
  M.setFromTriplets(MTriplets.begin(), MTriplets.end());

  // pre-factor system matrix
  SparseMatrix A = M + h2 * L;
  system_matrix.compute(A);
}

void MassSpringSolver::globalStep() {
  float h2 = system->time_step * system->time_step; // shorthand

  // compute right hand side
  VectorXf b = inertial_term + h2 * J * spring_directions + h2 * system->fext;

  // solve system and update state
  current_state = system_matrix.solve(b);
}

void MassSpringSolver::localStep() {
  unsigned int j = 0;
  for (Edge &i : system->spring_list) {
    Vector3f p12(
        current_state[3 * i.first + 0] - current_state[3 * i.second + 0],
        current_state[3 * i.first + 1] - current_state[3 * i.second + 1],
        current_state[3 * i.first + 2] - current_state[3 * i.second + 2]);

    p12.normalize();
    spring_directions[3 * j + 0] = system->rest_lengths[j] * p12[0];
    spring_directions[3 * j + 1] = system->rest_lengths[j] * p12[1];
    spring_directions[3 * j + 2] = system->rest_lengths[j] * p12[2];
    j++;
  }
}

void MassSpringSolver::solve(unsigned int n) {
  float a = system->damping_factor; // shorthand

  // update inertial term
  inertial_term = M * ((a + 1) * (current_state)-a * prev_state);

  // save current state in previous state
  prev_state = current_state;

  // perform steps
  for (unsigned int i = 0; i < n; i++) {
    localStep();
    globalStep();
  }
}

void MassSpringSolver::timedSolve(unsigned int ms) {
  // TODO
}
