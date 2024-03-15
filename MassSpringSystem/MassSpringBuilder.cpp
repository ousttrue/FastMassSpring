#include "MassSpringBuilder.h"
#include "MassSpringSystem.h"

void MassSpringBuilder::uniformGrid(
    unsigned int n,       // grid width
    float time_step,      // time step
    float rest_length,    // spring rest length (non-diagonal)
    float stiffness,      // spring stiffness
    float mass,           // node mass
    float damping_factor, // damping factor
    float gravity         // gravitationl force (-z axis)
) {
  // n must be odd
  assert(n % 2 == 1);

  // shorthand
  const double root2 = 1.41421356237;

  // compute n_points and n_springs
  unsigned int n_points = n * n;
  unsigned int n_springs = (n - 1) * (5 * n - 2);

  // build mass list
  VectorXf masses(mass * VectorXf::Ones(n_springs));

  // build spring list and spring parameters
  EdgeList spring_list(n_springs);
  structI.reserve(2 * (n - 1) * n);
  shearI.reserve(2 * (n - 1) * (n - 1));
  bendI.reserve(n * (n - 1));

  VectorXf rest_lengths(n_springs);
  VectorXf stiffnesses(n_springs);
  unsigned int k = 0; // spring counter
  for (unsigned int i = 0; i < n; i++) {
    for (unsigned int j = 0; j < n; j++) {
      // bottom right corner
      if (i == n - 1 && j == n - 1) {
        continue;
      }

      if (i == n - 1) {
        // structural spring
        spring_list[k] = Edge(n * i + j, n * i + j + 1);
        rest_lengths[k] = rest_length;
        stiffnesses[k] = stiffness;
        structI.push_back(k++);

        // bending spring
        if (j % 2 == 0) {
          spring_list[k] = Edge(n * i + j, n * i + j + 2);
          rest_lengths[k] = 2 * rest_length;
          stiffnesses[k] = stiffness;
          bendI.push_back(k++);
        }
        continue;
      }

      // right edge
      if (j == n - 1) {
        // structural spring
        spring_list[k] = Edge(n * i + j, n * (i + 1) + j);
        rest_lengths[k] = rest_length;
        stiffnesses[k] = stiffness;
        structI.push_back(k++);

        // bending spring
        if (i % 2 == 0) {
          spring_list[k] = Edge(n * i + j, n * (i + 2) + j);
          rest_lengths[k] = 2 * rest_length;
          stiffnesses[k] = stiffness;
          bendI.push_back(k++);
        }
        continue;
      }

      // structural springs
      spring_list[k] = Edge(n * i + j, n * i + j + 1);
      rest_lengths[k] = rest_length;
      stiffnesses[k] = stiffness;
      structI.push_back(k++);

      spring_list[k] = Edge(n * i + j, n * (i + 1) + j);
      rest_lengths[k] = rest_length;
      stiffnesses[k] = stiffness;
      structI.push_back(k++);

      // shearing springs
      spring_list[k] = Edge(n * i + j, n * (i + 1) + j + 1);
      rest_lengths[k] = root2 * rest_length;
      stiffnesses[k] = stiffness;
      shearI.push_back(k++);

      spring_list[k] = Edge(n * (i + 1) + j, n * i + j + 1);
      rest_lengths[k] = root2 * rest_length;
      stiffnesses[k] = stiffness;
      shearI.push_back(k++);

      // bending springs
      if (j % 2 == 0) {
        spring_list[k] = Edge(n * i + j, n * i + j + 2);
        rest_lengths[k] = 2 * rest_length;
        stiffnesses[k] = stiffness;
        bendI.push_back(k++);
      }
      if (i % 2 == 0) {
        spring_list[k] = Edge(n * i + j, n * (i + 2) + j);
        rest_lengths[k] = 2 * rest_length;
        stiffnesses[k] = stiffness;
        bendI.push_back(k++);
      }
    }
  }

  // compute external forces
  VectorXf fext = Vector3f(0, 0, -gravity).replicate(n_points, 1);

  result = std::make_shared<mass_spring_system>(
      n_points, n_springs, time_step, spring_list, rest_lengths, stiffnesses,
      masses, fext, damping_factor);
}
