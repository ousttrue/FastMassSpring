#pragma once
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <vector>

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
  std::shared_ptr<OpenMesh::TriMesh_ArrayKernelT<>> _mesh;
  std::vector<uint32_t> _indices;

  // vertex, normal, texutre, index
  std::shared_ptr<class Vao> _vao;

  App(float w, int n);
  ~App();
  void drawCloth(const glm::mat4 &proj, const glm::mat4 &view);
};
