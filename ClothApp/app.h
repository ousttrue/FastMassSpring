#pragma once
#include <glm/gtc/matrix_transform.hpp>

// System parameters
struct SystemParam {
  const int n = 33;     // must be odd, n * n = n_vertices | 61
  const float w = 2.0f; // width | 2.0f
  const float h =
      0.008f; // time step, smaller for better results | 0.008f = 0.016f/2
  const float r = w / (n - 1) * 1.05f; // spring rest legnth
  const float k = 1.0f;                // spring stiffness | 1.0f;
  const float m = 0.25f / (n * n);     // point mass | 0.25f
  const float a = 0.993f;              // damping, close to 1.0 | 0.993f
  const float g = 9.8f * m;            // gravitational force | 9.8f
};

// draw cloth function
class Demo {
  // Shader Handles
  class PhongShader *g_phongShader = nullptr; // linked phong shader
  class PickShader *g_pickShader = nullptr;   // linked pick shader

  // User Interaction
  class Renderer *g_pickRenderer = nullptr;

  // Shader parameters
  const glm::vec3 g_albedo = {0.0f, 0.3f, 0.7f};
  const glm::vec3 g_ambient = {0.01f, 0.01f, 0.01f};
  const glm::vec3 g_light = {1.0f, 1.0f, -1.0f};

  // Mesh
  class Mesh *g_clothMesh = nullptr; // halfedge data structure

  // Render Target
  class ProgramInput *g_render_target =
      nullptr; // vertex, normal, texutre, index

  // Mass Spring System
  class mass_spring_system *g_system = nullptr;
  class MassSpringSolver *g_solver = nullptr;

  // Constraint Graph
  class CgRootNode *g_cgRootNode = nullptr;

public:
  class UserInteraction *UI = nullptr;
  Demo(const SystemParam &param);
  ~Demo();
  void drawCloth(const glm::mat4 &proj, const glm::mat4 &view);
};
