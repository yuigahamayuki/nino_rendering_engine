#pragma once

#include "window.h"

class GLFWwindow;

namespace motion_animation {
  
class GLWindow : public Window {
public:
  GLWindow(uint32_t width, uint32_t height);
  ~GLWindow();

  void Loop(Renderer* renderer) override;

private:
  void Initialize();

  GLFWwindow* window_;
};  // class GLFWWindow

}  // namespace motion_animation