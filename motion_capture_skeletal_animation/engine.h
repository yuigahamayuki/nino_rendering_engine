#pragma once

#include <memory>
#include <string>

#include "window.h"

namespace motion_animation {

class Engine {
public:
  Engine() = default;
  Engine(uint32_t width, uint32_t height) : width_(width), height_(height) {

  }
  ~Engine() = default;

  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;

  void SetRendererToOpenGL();

  // For real game engine: read the save data to decide which scene to load.
  void LoadScene();

  void Run();

private:
  uint32_t width_;
  uint32_t height_;

  std::unique_ptr<Window> window_ptr_;

  std::string current_scene_name_;

};  // class Engine

}  // namespace motion_animation