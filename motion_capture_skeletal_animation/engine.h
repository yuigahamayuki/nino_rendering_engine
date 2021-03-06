#pragma once

#include <memory>
#include <string>

#include "window.h"
#include "renderer.h"

namespace motion_animation {

class Engine {
public:
  Engine() = default;
  Engine(uint32_t width, uint32_t height);
  ~Engine() = default;

  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;

  void SetRendererToOpenGL();

  // TODO(wushiyuan): For a real game engine: read the save data to decide which scene to load.
  void LoadScene();

  void Run();

private:
  uint32_t width_ = 0;
  uint32_t height_ = 0;

  std::string current_scene_name_;

  std::unique_ptr<Window> window_ptr_;

  std::unique_ptr<Renderer> renderer_ptr_;

};  // class Engine

}  // namespace motion_animation