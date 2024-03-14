#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

// draw cloth function
class App {
  // phong shader
  std::shared_ptr<class PhongMaterial> _material;

  // Shader parameters
  const glm::vec3 g_albedo = {0.0f, 0.3f, 0.7f};
  const glm::vec3 g_ambient = {0.01f, 0.01f, 0.01f};
  const glm::vec3 g_light = {1.0f, 1.0f, -1.0f};

public:
  // Mesh
  // halfedge data structure
  std::shared_ptr<class Mesh> _mesh;

  // vertex, normal, texutre, index
  std::shared_ptr<class Vao> _vao;

  App(const struct SystemParam &param);
  ~App();
  void drawCloth(const glm::mat4 &proj, const glm::mat4 &view);
};
