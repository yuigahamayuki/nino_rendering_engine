#include "GLFW_window.h"

#include <iostream>
#include <map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "input_manager.h"

namespace {

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  // make sure the viewport matches the new window dimensions; note that width and 
  // height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}

static const std::map<int, motion_animation::InputManager::KeyboardKey> kGLFWKeyMap {
  {GLFW_KEY_W, motion_animation::InputManager::KeyboardKey::w},
  {GLFW_KEY_S, motion_animation::InputManager::KeyboardKey::s},
  {GLFW_KEY_A, motion_animation::InputManager::KeyboardKey::a},
  {GLFW_KEY_D, motion_animation::InputManager::KeyboardKey::d},
  {GLFW_KEY_UP, motion_animation::InputManager::KeyboardKey::up},
  {GLFW_KEY_DOWN, motion_animation::InputManager::KeyboardKey::down},
  {GLFW_KEY_LEFT, motion_animation::InputManager::KeyboardKey::left},
  {GLFW_KEY_RIGHT, motion_animation::InputManager::KeyboardKey::right},
};

void ProcessInputKey(GLFWwindow* window, int key) {
  // Note(wushiyuan): If a user is not pressing the key, the state of NOT_PRESSED will be continuously set.
  motion_animation::InputManager::GetSharedInstance().SetKeyPressStateOfKey(kGLFWKeyMap.at(key), glfwGetKey(window, key) == GLFW_PRESS);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  for (const auto& key_map_pair : kGLFWKeyMap) {
    ProcessInputKey(window, key_map_pair.first);
  }
}

}  // namespace

namespace motion_animation {

GLWindow::GLWindow(uint32_t width, uint32_t height) : Window(width, height) {
  Initialize();
}

GLWindow::~GLWindow() {
  glfwTerminate();
}

void GLWindow::Loop(Renderer* renderer) {
  if (!window_) {
    return;
  }
  if (!renderer) {
    return;
  }
  // render loop
  // -----------
  while (!glfwWindowShouldClose(window_))
  {
    // input
    // -----
    processInput(window_);

    // render
    // ------
    renderer->Render();

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window_);
    glfwPollEvents();
  }
}

void GLWindow::Initialize() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window_ = glfwCreateWindow(width_, height_, "Render Api: OpenGL", NULL, NULL);
  if (window_ == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return;
  }
  glfwMakeContextCurrent(window_);
  glfwSetFramebufferSizeCallback(window_, framebuffer_size_callback);

  // glad: load all OpenGL function pointers
  // ---------------------------------------
  // Note(wushiyuan): need to call this api before calling any OpenGL apis such as glGenVertexArrays
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return;
  }
}

}  // namespace motion_animation