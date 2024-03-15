#include "Vao.h"

Vao::Vao() {
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

Vao::~Vao() {
  glDeleteBuffers(4, _vbo);
  glDeleteVertexArrays(1, &_handle);
}

void Vao::setIndexData(unsigned int *buff, unsigned int len) {
  this->n_elements = len;
  bufferData(3, buff, sizeof(unsigned int) * len);
}

void Vao::draw() {
  glBindVertexArray(_handle);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  glDrawElements(GL_TRIANGLES, n_elements, GL_UNSIGNED_INT, 0);
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
  glBindVertexArray(0);
}
