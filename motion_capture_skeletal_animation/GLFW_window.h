#pragma once

#include "window.h"

namespace motion_animation {
  
class GLFWWindow : public Window {
public:
  GLFWWindow(uint32_t width, uint32_t height);
  ~GLFWWindow();

  void Loop() override;

private:

};  // class GLFWWindow

}  // namespace motion_animation