#include "GLFW_window.h"

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace {

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  // make sure the viewport matches the new window dimensions; note that width and 
  // height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
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

  window_ = glfwCreateWindow(width_, height_, "LearnOpenGL", NULL, NULL);
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