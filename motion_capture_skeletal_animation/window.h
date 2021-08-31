#pragma once

#include <cstdint>

namespace motion_animation {

class Window {
 public:
  Window(uint32_t width, uint32_t height) : width_(width), height_(height) {

  }
  virtual ~Window() = default;

  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;

  uint32_t width() const {
    return width_;
  }

  uint32_t height() const {
    return height_;
  }

  virtual void Loop() = 0;

 protected:
  uint32_t width_;
  uint32_t height_;

};  // class Window

}  // namespace motion_animation