#pragma once

#include "scene.h"

namespace motion_animation {

class SceneManager {
public:
  static SceneManager& GetSharedInstance();

  ~SceneManager() = default;

  SceneManager(const SceneManager&) = delete;
  SceneManager& operator=(const SceneManager&) = delete;

  void AddScene(const std::string& scene_name);

private:
  SceneManager() = default;
  std::map<std::string, Scene> scenes_;

};  // class SceneManager

}  // namespace motion_animation