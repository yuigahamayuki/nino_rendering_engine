#pragma once

#include <map>

#include "model.h"
#include "camera.h"
#include "renderer.h"

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

  // Note(wushiyuan): For real game engine: read a config file for the scene, then load corresponding assets.
  void LoadSceneAssets(Renderer* renderer);

  const Camera& GetCamera() const {
    return camera_;
  }

 private:
   void LoadModels(Renderer* renderer);
   void LoadCamera();
   void LoadLights();

   void LoadSingleModel(const std::string& model_file_name, const std::string& model_name);

  std::string scene_name_;

  std::map<std::string, Model> models_;  // key: model_name value: Model object
  Camera camera_;
};  // class Scene

}  // namespace motion_animation