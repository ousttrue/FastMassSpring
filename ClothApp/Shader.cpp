#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

// GLSHADER
// ///////////////////////////////////////////////////////////////////////////////////
GLShader::GLShader(GLenum shaderType) : _handle(glCreateShader(shaderType)){};

GLShader::~GLShader() { glDeleteShader(_handle); }

void GLShader::compile(const std::string &source) {
  GLint compiled = 0; // Compiled flag
  const char *ptrs[] = {source.c_str()};
  const GLint lens[] = {static_cast<GLint>(source.size())};
  glShaderSource(_handle, 1, ptrs, lens);
  glCompileShader(_handle);
  glGetShaderiv(_handle, GL_COMPILE_STATUS, &compiled);
  if (compiled != GL_TRUE) {
    GLint logSize = 0;
    glGetShaderiv(_handle, GL_INFO_LOG_LENGTH, &logSize);
    std::vector<GLchar> errorLog(logSize);
    glGetShaderInfoLog(_handle, logSize, &logSize, &errorLog[0]);
    std::cerr << &errorLog[0] << std::endl;
    throw std::runtime_error("Failed to compile shader.");
  }
}

// GLPROGRAM
// //////////////////////////////////////////////////////////////////////////////////
GLProgram::GLProgram() : _handle(glCreateProgram()) {}

GLProgram::~GLProgram() { glDeleteProgram(_handle); }

std::shared_ptr<GLProgram> GLProgram::make(const std::string &vs_source,
                                           const std::string &fs_source) {
  GLShader vs(GL_VERTEX_SHADER);
  vs.compile(vs_source);

  GLShader fs(GL_FRAGMENT_SHADER);
  fs.compile(fs_source);

  auto shader = std::make_shared<GLProgram>();
  shader->link(vs, fs);
  return shader;
}

void GLProgram::link(const GLShader &vshader, const GLShader &fshader) {
  GLint linked = 0; // Linked flag
  glAttachShader(_handle, vshader.handle());
  glAttachShader(_handle, fshader.handle());
  glLinkProgram(_handle);
  glDetachShader(_handle, vshader.handle());
  glDetachShader(_handle, fshader.handle());
  glGetProgramiv(_handle, GL_LINK_STATUS, &linked);
  if (!linked)
    throw std::runtime_error("Failed to link shaders.");

  // get camera uniforms
  _uModelViewMatrix = glGetUniformLocation(_handle, "uModelViewMatrix");
  _uProjectionMatrix = glGetUniformLocation(_handle, "uProjectionMatrix");

  // post link
  postLink();
}

void GLProgram::postLink() {}

void GLProgram::setUniformMat4(GLuint unif, glm::mat4 m) {
  glUniformMatrix4fv(unif, 1, GL_FALSE, glm::value_ptr(m[0]));
}

void GLProgram::bind() { glUseProgram(_handle); }
void GLProgram::unbind() { glUseProgram(0); }

void GLProgram::setModelView(glm::mat4 m) {
  setUniformMat4(_uModelViewMatrix, m);
}

void GLProgram::setProjection(glm::mat4 m) {
  setUniformMat4(_uProjectionMatrix, m);
}

// SHADER PROGRAMS
// ////////////////////////////////////////////////////////////////////////////
PhongMaterial::PhongMaterial() {}

void PhongMaterial::setAlbedo(const glm::vec3 &albedo) {
  assert(_uAlbedo >= 0);
  glUseProgram(_shader->handle());
  glUniform3f(_uAlbedo, albedo[0], albedo[1], albedo[2]);
  glUseProgram(0);
}
void PhongMaterial::setAmbient(const glm::vec3 &ambient) {
  assert(_uAmbient >= 0);
  glUseProgram(_shader->handle());
  glUniform3f(_uAmbient, ambient[0], ambient[1], ambient[2]);
  glUseProgram(0);
}
void PhongMaterial::setLight(const glm::vec3 &light) {
  assert(_uLight >= 0);
  glUseProgram(_shader->handle());
  glUniform3f(_uLight, light[0], light[1], light[2]);
  glUseProgram(0);
}

std::shared_ptr<PhongMaterial> PhongMaterial::make() {
  auto ibasic = readall("./ClothApp/shaders/basic.vshader");
  assert(ibasic.size());
  auto iphong = readall("./ClothApp/shaders/phong.fshader");
  assert(iphong.size());
  auto shader = GLProgram::make(ibasic, iphong);

  auto material = std::shared_ptr<PhongMaterial>(new PhongMaterial);

  material->_shader = shader;

  // get uniforms
  material->_uAlbedo = glGetUniformLocation(shader->handle(), "uAlbedo");
  material->_uAmbient = glGetUniformLocation(shader->handle(), "uAmbient");
  material->_uLight = glGetUniformLocation(shader->handle(), "uLight");

  return material;
}

PickMaterial::PickMaterial() {}

std::shared_ptr<PickMaterial> PickMaterial::make() {
  auto ibasic = readall("./ClothApp/shaders/basic.vshader");
  assert(ibasic.size());
  auto ifrag = readall("./ClothApp/shaders/pick.fshader");
  assert(ifrag.size());
  auto shader = GLProgram::make(ibasic, ifrag);

  auto material = std::shared_ptr<PickMaterial>(new PickMaterial);
  material->_shader = shader;

  // get uniforms
  material->_uTessFact = glGetUniformLocation(shader->handle(), "uTessFact");
  return material;
}

void PickMaterial::setTessFact(unsigned int n) {
  assert(_uTessFact >= 0);
  glUseProgram(_shader->handle());
  glUniform1i(_uTessFact, n);
  glUseProgram(0);
}
