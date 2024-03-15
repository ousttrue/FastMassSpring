#include <GL/glew.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#include "Mesh.h"
#include "Shader.h"
#include "UserInteraction.h"
#include "Vao.h"
#include "app.h"
#include "param.h"
#include "pickcallback.h"

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

  if (!glfwInit()) {
    exit(EXIT_FAILURE);
  }

  // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
  // GL ES 2.0 + GLSL 100
  const char *glsl_version = "#version 100";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
  // GL 3.2 + GLSL 150
  const char *glsl_version = "#version 150";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required on Mac
#else
  // GL 3.0 + GLSL 130
  const char *glsl_version = "#version 130";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+
  // only glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // 3.0+ only
#endif

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

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  // ImGui::StyleColorsLight();

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  g_app = std::make_shared<App>(param.w, param.n);

  auto g_pickShader = PickMaterial::make();
  g_pickShader->setTessFact(param.n);

  PickCallback callback = [_shader = g_pickShader->shader(),
                           _vao = g_app->_vao](const glm::mat4 &p,
                                               const glm::mat4 &mv, int mouse_x,
                                               int mouse_y) -> color {
    // render scene
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_FRAMEBUFFER_SRGB);

    _shader->bind();
    _shader->setModelView(mv);
    _shader->setProjection(p);
    _vao->draw();
    _shader->unbind();

    glFlush();
    color c(3);
    glReadPixels(mouse_x, mouse_y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &c[0]);

    // return to normal state
    glClearColor(0.25f, 0.25f, 0.25f, 0);
    glEnable(GL_FRAMEBUFFER_SRGB);

    return c;
  };

  g_demo = std::make_shared<DEMO>(callback, param, g_app->_mesh->vbuff());

  glfwSetMouseButtonCallback(window, mouse_button_callback);
  glfwSetCursorPosCallback(window, cursor_position_callback);

  initCamera(param);
  while (!glfwWindowShouldClose(window)) {
    // update window event
    glfwPollEvents();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // 1. Show the big demo window (Most of the sample code is in
    // ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear
    // ImGui!).
    ImGui::ShowDemoWindow({});
    ImGui::Render();

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

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
  }

  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
