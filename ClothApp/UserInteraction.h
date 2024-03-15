#pragma once
#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <vector>

class UserInteraction {
protected:
  std::shared_ptr<class GLProgram> _shader;
  std::shared_ptr<class Vao> _vao;

  typedef glm::vec3 vec3;
  typedef std::vector<unsigned char> color;

  int i;                       // index of fixed point
  float *vbuff;                // vertex buffer
  class CgPointFixNode *fixer; // point fixer
  virtual int colorToIndex(color c) const = 0;

public:
  UserInteraction(const std::shared_ptr<GLProgram> &shader,
                  const std::shared_ptr<Vao> &vao, CgPointFixNode *fixer,
                  float *vbuff);
  virtual ~UserInteraction(){};

  void grabPoint(const glm::mat4 &p, const glm::mat4 &mv, int mouse_x,
                 int mouse_y); // grab point with color c
  void movePoint(vec3 v);      // move grabbed point along mouse
  void releasePoint();         // release grabbed point;
};

class GridMeshUI : public UserInteraction {
protected:
  const unsigned int n; // grid width
  virtual int colorToIndex(color c) const;

public:
  GridMeshUI(const std::shared_ptr<GLProgram> &shader,
             const std::shared_ptr<Vao> &vao, CgPointFixNode *fixer,
             float *vbuff, unsigned int n);
};
