#pragma once
#include "noncopyable.h"
#include <GL/glew.h>
#include <fstream>
#include <glm/glm.hpp>
#include <memory>

class GLShader : public NonCopyable {
  GLuint _handle;

public:
  GLShader(GLenum shaderType);
  ~GLShader();
  std::shared_ptr<GLShader> make(const std::string &source);
  GLuint handle() const { return _handle; }
  void compile(const std::string &source);
};

class GLProgram : public NonCopyable {
  GLuint _handle;
  GLuint _uModelViewMatrix;
  GLuint _uProjectionMatrix;

public:
  GLProgram();
  virtual ~GLProgram();
  GLuint handle() const { return _handle; }
  static std::shared_ptr<GLProgram> make(const std::string &vs,
                                         const std::string &fs);
  void bind();
  void unbind();
  void setModelView(glm::mat4 m);
  void setProjection(glm::mat4 m);

private:
  void setUniformMat4(GLuint unif, glm::mat4 m);
  virtual void link(const GLShader &vshader, const GLShader &fshader);
  virtual void postLink();
};

class PhongMaterial : public NonCopyable {
  GLuint _uAlbedo = -1;
  GLuint _uAmbient = -1;
  GLuint _uLight = -1;
  std::shared_ptr<GLProgram> _shader;

public:
  PhongMaterial();
  static std::shared_ptr<PhongMaterial> make();
  std::shared_ptr<GLProgram> shader() const { return _shader; }
  void setAlbedo(const glm::vec3 &albedo);
  void setAmbient(const glm::vec3 &ambient);
  void setLight(const glm::vec3 &light);
};

class PickMaterial : public NonCopyable {
  GLuint _uTessFact;
  std::shared_ptr<GLProgram> _shader;

public:
  PickMaterial();
  static std::shared_ptr<PickMaterial> make();
  std::shared_ptr<GLProgram> shader() const { return _shader; }
  void setTessFact(unsigned int n);
};

inline std::string readall(const std::string &path) {
  std::ifstream source(path, std::ios::binary);
  std::string text;
  source.seekg(0, std::ios_base::end);
  std::streampos fileSize = source.tellg();
  text.resize(fileSize);
  source.seekg(0, std::ios_base::beg);
  source.read((char *)text.data(), fileSize);
  return text;
}
