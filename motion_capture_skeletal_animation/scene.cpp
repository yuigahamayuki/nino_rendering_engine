#include "scene.h"

#include "model_importer.h"

namespace motion_animation {

void Scene::LoadSceneAssets() {
  LoadModels();
  LoadCamera();
  LoadLights();
}

void Scene::LoadModels() {
  const std::string model_file_name("D:/game_assets/models/ruby-rose/source/rubyAnimated002.fbx");
  if (ModelImporter::GetSharedInstance().LoadModelFile(model_file_name)) {
    const std::string model_name("ruby");
    models_.emplace(model_name, Model(model_name));
  }
}

void Scene::LoadCamera() {
}

void Scene::LoadLights() {
}

}  // namespace motion_animation