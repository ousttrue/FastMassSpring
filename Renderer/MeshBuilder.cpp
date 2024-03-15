#include "MeshBuilder.h"

IndexedBuffer uniformGrid(float w, int n) {
  auto mesh = std::make_shared<OpenMesh::TriMesh_ArrayKernelT<>>();
  unsigned int ibuffLen = 6 * (n - 1) * (n - 1);
  std::vector<unsigned int> ibuff(ibuffLen);

  // request mesh properties
  mesh->request_vertex_normals();
  mesh->request_vertex_normals();
  mesh->request_vertex_texcoords2D();

  // generate mesh
  unsigned int idx = 0;            // vertex index
  const float d = w / (n - 1);     // step distance
  const float ud = 1.0f / (n - 1); // unit step distance
  const OpenMesh::Vec3f o =
      OpenMesh::Vec3f(-w / 2.0f, w / 2.0f, 0.0f); // origin
  const OpenMesh::Vec3f ux =
      OpenMesh::Vec3f(1.0f, 0.0f, 0.0f); // unit x direction
  const OpenMesh::Vec3f uy =
      OpenMesh::Vec3f(0.0f, -1.0f, 0.0f); // unit y direction
  std::vector<OpenMesh::VertexHandle> handle_table(
      n * n); // table storing vertex handles for easy grid connectivity
              // establishment

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      handle_table[j + i * n] =
          mesh->add_vertex(o + d * j * ux + d * i * uy); // add vertex
      mesh->set_texcoord2D(
          handle_table[j + i * n],
          OpenMesh::Vec2f(ud * j, ud * i)); // add texture coordinates

      // add connectivity
      if (i > 0 && j < n - 1) {
        mesh->add_face(handle_table[j + i * n],
                       handle_table[j + 1 + (i - 1) * n],
                       handle_table[j + (i - 1) * n]);

        ibuff[idx++] = j + i * n;
        ibuff[idx++] = j + 1 + (i - 1) * n;
        ibuff[idx++] = j + (i - 1) * n;
      }

      if (j > 0 && i > 0) {
        mesh->add_face(handle_table[j + i * n], handle_table[j + (i - 1) * n],
                       handle_table[j - 1 + i * n]);

        ibuff[idx++] = j + i * n;
        ibuff[idx++] = j + (i - 1) * n;
        ibuff[idx++] = j - 1 + i * n;
      }
    }
  }

  // calculate normals
  mesh->request_face_normals();
  mesh->update_normals();
  mesh->release_face_normals();

  return {mesh, ibuff};
}
