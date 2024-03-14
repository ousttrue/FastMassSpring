#pragma once
#include <glm/gtc/matrix_transform.hpp>

// draw cloth function
class App {
  // Shader Handles
  class PhongShader *g_phongShader = nullptr; // linked phong shader

  // Shader parameters
  const glm::vec3 g_albedo = {0.0f, 0.3f, 0.7f};
  const glm::vec3 g_ambient = {0.01f, 0.01f, 0.01f};
  const glm::vec3 g_light = {1.0f, 1.0f, -1.0f};

public:
  // Mesh
  class Mesh *g_clothMesh = nullptr; // halfedge data structure

  // Render Target
  class ProgramInput *g_render_target =
      nullptr; // vertex, normal, texutre, index

  App(const struct SystemParam &param);
  ~App();
  void drawCloth(const glm::mat4 &proj, const glm::mat4 &view);
};
