#pragma once
#include "noncopyable.h"
#include <GL/glew.h>

class Vao : public NonCopyable {
  GLuint _handle; // vertex array object handle
  GLuint _vbo[4]; // vertex buffer object handles | position, normal, texture,
                  // index

  unsigned int n_elements;

public:
  Vao();
  ~Vao();
  void setIndexData(unsigned int *buff, unsigned int len);

  template <typename T>
  void bufferData(unsigned int index, const T *buff, size_t count) {
    glBindBuffer(GL_ARRAY_BUFFER, _vbo[index]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(T) * count, buff, GL_STATIC_DRAW);
  }

  // void Vao::setPositionData(float *buff, unsigned int len) {
  //   bufferData(0, buff, sizeof(float) * len);
  // }
  //
  // void Vao::setNormalData(float *buff, unsigned int len) {
  //   bufferData(1, buff, sizeof(float) * len);
  // }
  //
  // void Vao::setTextureData(float *buff, unsigned int len) {
  //   bufferData(2, buff, sizeof(float) * len);
  // }

  GLuint handle() const { return _handle; }
  void draw();
};
