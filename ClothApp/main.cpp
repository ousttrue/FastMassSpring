#include <GL/glew.h>

#include "UserInteraction.h"
#include "app.h"
#include "param.h"
// #include <GL/freeglut.h>
#include <GLFW/glfw3.h>

std::shared_ptr<App> g_app;
#if 0
#include "demo_hang.h"
using DEMO = DemoHang;
#else
#include "demo_drop.h"
using DEMO = DemoDrop;
#endif
std::shared_ptr<DEMO> g_demo;

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

void mouse_button_callback(GLFWwindow *window, int button, int action,
                           int mods) {
  g_mouseLClickButton |=
      (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS);
  g_mouseRClickButton |=
      (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS);
  g_mouseMClickButton |=
      (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS);

  g_mouseLClickButton &=
      !(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE);
  g_mouseRClickButton &=
      !(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE);
  g_mouseMClickButton &=
      !(button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE);

  g_mouseClickDown =
      g_mouseLClickButton || g_mouseRClickButton || g_mouseMClickButton;

  // TODO: move to UserInteraction class: add renderer member variable
  // pick point
  if (g_mouseLClickButton) {
    g_demo->UI->grabPoint(g_ProjectionMatrix, g_ModelViewMatrix, g_mouseClickX,
                          g_mouseClickY);
  } else {
    g_demo->UI->releasePoint();
  }
}

static void cursor_position_callback(GLFWwindow *window, double x, double y) {
  const float dx = float(x - g_mouseClickX);
  const float dy = float(-(g_windowHeight - y - 1 - g_mouseClickY));
  if (g_mouseLClickButton) {
    // glm::vec3 ux(g_ModelViewMatrix * glm::vec4(1, 0, 0, 0));
    // glm::vec3 uy(g_ModelViewMatrix * glm::vec4(0, 1, 0, 0));
    glm::vec3 ux(0, 1, 0);
    glm::vec3 uy(0, 0, -1);
    g_demo->UI->movePoint(0.01f * (dx * ux + dy * uy));
  }

  g_mouseClickX = static_cast<int>(x);
  g_mouseClickY = g_windowHeight - static_cast<int>(y) - 1;
}

static void error_callback(int error, const char *description) {
  fprintf(stderr, "Error: %s\n", description);
}

int main(int argc, char **argv) {
  SystemParam param = {};

  glfwSetErrorCallback(error_callback);

  if (!glfwInit())
    exit(EXIT_FAILURE);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  auto window =
      glfwCreateWindow(g_windowWidth, g_windowHeight, "Cloth App", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return 1;
  }
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  GLenum err = glewInit();
  if (GLEW_OK != err) {
    /* Problem: glewInit failed, something is seriously wrong. */
    fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    return 2;
  }

  g_app = std::make_shared<App>(param);
  g_demo = std::make_shared<DEMO>(param, g_app->_mesh, g_app->_vao);

  glfwSetMouseButtonCallback(window, mouse_button_callback);
  glfwSetCursorPosCallback(window, cursor_position_callback);

  initCamera(param);
  while (!glfwWindowShouldClose(window)) {
    // update window event
    glfwPollEvents();
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    if (width != g_windowWidth || height != g_windowHeight) {
      g_windowWidth = width;
      g_windowHeight = height;
      updateProjection();
    }

    // update
    g_demo->Animation();

    // draw
    glViewport(0, 0, width, height);
    g_app->drawCloth(g_ProjectionMatrix, g_ModelViewMatrix);
    glfwSwapBuffers(window);
  }

  return 0;
}
