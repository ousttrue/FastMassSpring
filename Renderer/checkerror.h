#pragma once
#include <GL/GL.h>
#include <GL/GLU.h>
#include <iostream>
#include <stdexcept>
#include <string>

inline void checkGlErrors() {
  const GLenum errCode = glGetError();
  if (errCode != GL_NO_ERROR) {
    std::string error("GL Error: ");
    error += reinterpret_cast<const char *>(gluErrorString(errCode));
    std::cerr << error << std::endl;
    throw std::runtime_error(error);
  }
}
