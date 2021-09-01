#include "engine.h"

#include "GLFW_window.h"
#include "GL_renderer.h"
#include "scene_manager.h"

namespace motion_animation {

void Engine::SetRendererToOpenGL() {
  window_ptr_ = std::make_unique<GLFWWindow>(width_, height_);
  renderer_ptr_ = std::make_unique<GLRenderer>();
}

void Engine::LoadScene()
{
  const std::string scene_name("test scene");
  SceneManager::GetSharedInstance().AddScene(scene_name, renderer_ptr_.get());
}

void Engine::Run() {
  if (window_ptr_) {
    window_ptr_->Loop();
  }
}

}