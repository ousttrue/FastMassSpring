#pragma once
#include "Shader.h"
#include "Vao.h"
#include <glm/gtc/matrix_transform.hpp>

class Renderer {
protected:
  std::shared_ptr<GLProgram> program;
  ProgramInput *input;
  unsigned int n_elements;

public:
  Renderer();

  void setProgram(const std::shared_ptr<GLProgram> &program) {
    this->program = program;
  }

  void setProgramInput(ProgramInput *input);
  void setModelview(const glm::mat4 &mv);
  void setProjection(const glm::mat4 &p);
  void setElementCount(unsigned int n_elements);

  void draw();
};
