#pragma once
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <vector>

class Mesh : public OpenMesh::TriMesh_ArrayKernelT<> {
  std::vector<unsigned int> _ibuff;

public:
  static std::shared_ptr<Mesh> uniformGrid(float w, int n);

  // pointers to buffers
  float *vbuff() { return (float *)&(this->point(*this->vertices_begin())); }
  float *nbuff() { return (float *)&(this->normal(*this->vertices_begin())); }
  float *tbuff() {
    return (float *)&(this->texcoord2D(*this->vertices_begin()));
  }
  unsigned int *ibuff() { return &_ibuff[0]; }

  // buffer sizes
  unsigned int vbuffLen() { return (unsigned int)n_vertices() * 3; }
  unsigned int nbuffLen() { return (unsigned int)n_vertices() * 3; }
  unsigned int tbuffLen() { return (unsigned int)n_vertices() * 2; }
  unsigned int ibuffLen() { return (unsigned int)_ibuff.size(); }

  // set index buffer
  void useIBuff(std::vector<unsigned int> &_ibuff) { this->_ibuff = _ibuff; }
};
