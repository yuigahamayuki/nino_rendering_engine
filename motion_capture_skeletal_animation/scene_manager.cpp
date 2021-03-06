#include "scene_manager.h"

namespace motion_animation {

SceneManager& SceneManager::GetSharedInstance() {
  static SceneManager scene_manager;
  return scene_manager;
}

void SceneManager::AddScene(const std::string& scene_name, Renderer* renderer) {
  scenes_.emplace(scene_name, Scene(scene_name));
  scenes_[scene_name].LoadSceneAssets(renderer);

  current_scene_name_ = scene_name;
}

}  // namespace motion_animation