#include <GL/glew.h>

#include "UserInteraction.h"
#include "app.h"
#include <GL/freeglut.h>

// Window
static int g_windowWidth = 640;
static int g_windowHeight = 640;

// Animation
static const int g_fps = 60; // frames per second  | 60
static const int g_frame_time =
    15; // approximate time for frame calculations | 15
static const int g_animation_timer =
    (int)((1.0f / g_fps) * 1000 - g_frame_time);

static bool g_mouseClickDown = false;
static bool g_mouseLClickButton;
static bool g_mouseRClickButton;
static bool g_mouseMClickButton;
static int g_mouseClickX;
static int g_mouseClickY;

// Scene matrices
static glm::mat4 g_ProjectionMatrix;
static glm::mat4 g_ModelViewMatrix;

// Scene parameters
static const float g_camera_distance = 4.2f;

// Constants
static const float PI = glm::pi<float>();

static void updateProjection() {
  g_ProjectionMatrix = glm::perspective(
      PI / 4.0f, g_windowWidth * 1.0f / g_windowHeight, 0.01f, 1000.0f);
}

static void initCamera(const SystemParam &param) {
  g_ModelViewMatrix =
      glm::lookAt(glm::vec3(0.618, -0.786, 0.3f) * g_camera_distance,
                  glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 1.0f)) *
      glm::translate(glm::mat4(1), glm::vec3(0.0f, 0.0f, param.w / 4));
  updateProjection();
}

void reshape(int w, int h) {
  g_windowWidth = w;
  g_windowHeight = h;
  glViewport(0, 0, w, h);
  updateProjection();
}

std::shared_ptr<Demo> g_tmp;

void mouse(const int button, const int state, const int x, const int y) {
  g_mouseClickX = x;
  g_mouseClickY = g_windowHeight - y - 1;

  g_mouseLClickButton |= (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN);
  g_mouseRClickButton |= (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN);
  g_mouseMClickButton |= (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN);

  g_mouseLClickButton &= !(button == GLUT_LEFT_BUTTON && state == GLUT_UP);
  g_mouseRClickButton &= !(button == GLUT_RIGHT_BUTTON && state == GLUT_UP);
  g_mouseMClickButton &= !(button == GLUT_MIDDLE_BUTTON && state == GLUT_UP);

  g_mouseClickDown =
      g_mouseLClickButton || g_mouseRClickButton || g_mouseMClickButton;

  // TODO: move to UserInteraction class: add renderer member variable
  // pick point
  if (g_mouseLClickButton) {
    g_tmp->UI->setModelview(g_ModelViewMatrix);
    g_tmp->UI->setProjection(g_ProjectionMatrix);
    g_tmp->UI->grabPoint(g_mouseClickX, g_mouseClickY);
  } else {
    g_tmp->UI->releasePoint();
  }
}

void display() {
  //
  g_tmp->drawCloth(g_ProjectionMatrix, g_ModelViewMatrix);
}

void motion(const int x, const int y) {
  const float dx = float(x - g_mouseClickX);
  const float dy = float(-(g_windowHeight - y - 1 - g_mouseClickY));

  if (g_mouseLClickButton) {
    // glm::vec3 ux(g_ModelViewMatrix * glm::vec4(1, 0, 0, 0));
    // glm::vec3 uy(g_ModelViewMatrix * glm::vec4(0, 1, 0, 0));
    glm::vec3 ux(0, 1, 0);
    glm::vec3 uy(0, 0, -1);
    g_tmp->UI->movePoint(0.01f * (dx * ux + dy * uy));
  }

  g_mouseClickX = x;
  g_mouseClickY = g_windowHeight - y - 1;
}

int main(int argc, char **argv) {
  SystemParam param = {};

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(g_windowWidth, g_windowHeight);
  glutCreateWindow("Cloth App");
  glewInit();
  g_tmp = std::make_shared<Demo>(param);

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);

  initCamera(param);
  while (true) {
    glutMainLoopEvent();
    glutPostRedisplay();
    glutSwapBuffers();
  }
  return 0;
}
