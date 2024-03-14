#pragma once
#include "noncopyable.h"
#include <GL/glew.h>

class Vao : public NonCopyable {
  GLuint _handle; // vertex array object handle
  GLuint _vbo[4]; // vertex buffer object handles | position, normal, texture,
                  // index
  void bufferData(unsigned int index, void *buff, size_t size);
  unsigned int n_elements;

public:
  Vao();
  ~Vao();
  void setPositionData(float *buff, unsigned int len);
  void setNormalData(float *buff, unsigned int len);
  void setTextureData(float *buff, unsigned int len);
  void setIndexData(unsigned int *buff, unsigned int len);
  GLuint handle() const { return _handle; }
  void draw();
};
