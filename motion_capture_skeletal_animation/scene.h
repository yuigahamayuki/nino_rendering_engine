#pragma once

#include <map>

#include "model.h"

namespace motion_animation {

// A Scene object has camera, lights, models.
class Scene {
 public:
  Scene() = default;
  Scene(const std::string& scene_name) : scene_name_(scene_name) {

  }
  ~Scene() = default;

  Scene(const Scene&) = delete;
  Scene& operator=(const Scene&) = delete;

  Scene(Scene&&) = default;
  Scene& operator=(Scene&&) = default;

  // For real game engine: read a config file for the scene, then load corresponding assets.
  void LoadSceneAssets();

 private:
   void LoadModels();
   void LoadCamera();
   void LoadLights();

  std::string scene_name_;

  std::map<std::string, Model> models_;  // key: model_name value: Model object
};  // class Scene

}  // namespace motion_animation