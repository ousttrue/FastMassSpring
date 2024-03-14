#include "Vao.h"

ProgramInput::ProgramInput() {
  // generate buffers
  glGenBuffers(4, &_vbo[0]);

  // generate vertex array object
  glGenVertexArrays(1, &_handle);
  glBindVertexArray(_handle);

  // positions
  glBindBuffer(GL_ARRAY_BUFFER, _vbo[0]);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

  // normals
  glBindBuffer(GL_ARRAY_BUFFER, _vbo[1]);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

  // texture coordinates
  glBindBuffer(GL_ARRAY_BUFFER, _vbo[2]);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

  // indices
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vbo[3]);

  glBindVertexArray(0);
}

void ProgramInput::bufferData(unsigned int index, void *buff, size_t size) {
  glBindBuffer(GL_ARRAY_BUFFER, _vbo[index]);
  glBufferData(GL_ARRAY_BUFFER, size, buff, GL_STATIC_DRAW);
}
void ProgramInput::setPositionData(float *buff, unsigned int len) {
  bufferData(0, buff, sizeof(float) * len);
}

void ProgramInput::setNormalData(float *buff, unsigned int len) {
  bufferData(1, buff, sizeof(float) * len);
}

void ProgramInput::setTextureData(float *buff, unsigned int len) {
  bufferData(2, buff, sizeof(float) * len);
}

void ProgramInput::setIndexData(unsigned int *buff, unsigned int len) {
  bufferData(3, buff, sizeof(unsigned int) * len);
}

ProgramInput::~ProgramInput() {
  glDeleteBuffers(4, _vbo);
  glDeleteVertexArrays(1, &_handle);
}
