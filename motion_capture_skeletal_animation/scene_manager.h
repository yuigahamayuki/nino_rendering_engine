#pragma once

#include "scene.h"
#include "renderer.h"

namespace motion_animation {

class SceneManager {
public:
  static SceneManager& GetSharedInstance();

  ~SceneManager() = default;

  SceneManager(const SceneManager&) = delete;
  SceneManager& operator=(const SceneManager&) = delete;

  void AddScene(const std::string& scene_name, Renderer* renderer);

  const Scene* GetCurrentScene() const {
    const Scene* scene_ptr = nullptr;
    if (scenes_.find(current_scene_name_) != scenes_.end()) {
      scene_ptr = &(scenes_.at(current_scene_name_));
    }

    return scene_ptr;
  }

private:
  SceneManager() = default;
  std::map<std::string, Scene> scenes_;

  std::string current_scene_name_;

};  // class SceneManager

}  // namespace motion_animation