#include "UserInteraction.h"
#include "MassSpringSolver.h"
#include <cmath>

UserInteraction::UserInteraction(const PickCallback &callback,
                                 CgPointFixNode *fixer, float *vbuff)
    : _callback(callback), i(-1), vbuff(vbuff), fixer(fixer) {}

void UserInteraction::grabPoint(const glm::mat4 &p, const glm::mat4 &mv,
                                int mouse_x, int mouse_y) {
  // read color
  auto c = _callback(p, mv, mouse_x, mouse_y);
  i = colorToIndex(c);
  if (i != -1) {
    fixer->fixPoint(i);
  }
}

void UserInteraction::releasePoint() {
  if (i == -1)
    return;
  fixer->releasePoint(i);
  i = -1;
}

void UserInteraction::movePoint(const glm::vec3 &v) {
  if (i == -1)
    return;
  fixer->releasePoint(i);
  for (int j = 0; j < 3; j++)
    vbuff[3 * i + j] += v[j];
  fixer->fixPoint(i);
}

GridMeshUI::GridMeshUI(const PickCallback &callback, CgPointFixNode *fixer,
                       float *vbuff, unsigned int n)
    : UserInteraction(callback, fixer, vbuff), n(n) {}

int GridMeshUI::colorToIndex(color c) const {
  if (c[2] != 51)
    return -1;
  int vx = static_cast<int>(std::round((n - 1) * c[0] / 255.0));
  int vy = static_cast<int>(std::round((n - 1) * c[1] / 255.0));
  return n * vy + vx;
}
