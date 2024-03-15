#pragma once
#include "pickcallback.h"

class UserInteraction {
public:
private:
  PickCallback _callback;

protected:
  int i;                       // index of fixed point
  float *vbuff;                // vertex buffer
  class CgPointFixNode *fixer; // point fixer
  virtual int colorToIndex(color c) const = 0;

public:
  UserInteraction(const PickCallback &callback, CgPointFixNode *fixer,
                  float *vbuff);
  virtual ~UserInteraction(){};

  void grabPoint(const glm::mat4 &p, const glm::mat4 &mv, int mouse_x,
                 int mouse_y);        // grab point with color c
  void movePoint(const glm::vec3 &v); // move grabbed point along mouse
  void releasePoint();                // release grabbed point;
};

class GridMeshUI : public UserInteraction {
protected:
  const unsigned int n; // grid width
  virtual int colorToIndex(color c) const;

public:
  GridMeshUI(const PickCallback &callback, CgPointFixNode *fixer, float *vbuff,
             unsigned int n);
};
