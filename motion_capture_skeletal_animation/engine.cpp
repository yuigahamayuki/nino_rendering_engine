#include "engine.h"

#include "GLFW_window.h"
#include "scene_manager.h"

namespace motion_animation {

void Engine::SetRendererToOpenGL() {
  window_ptr_ = std::make_unique<GLFWWindow>(width_, height_);
}

void Engine::LoadScene()
{
  const std::string scene_name("test scene");
  SceneManager::GetSharedInstance().AddScene(scene_name);
}

void Engine::Run() {
  if (window_ptr_) {
    window_ptr_->Loop();
  }
}

}