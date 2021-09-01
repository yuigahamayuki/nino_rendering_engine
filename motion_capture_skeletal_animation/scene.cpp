#include "scene.h"

#include "model_importer.h"
#include "assets.h"

namespace motion_animation {

void Scene::LoadSceneAssets(Renderer* renderer) {
  LoadModels(renderer);
  LoadCamera();
  LoadLights();
}

void Scene::LoadModels(Renderer* renderer) {
  // Note(wushiyuan): For a real game engine: read config file to determine which models are contained in the scene, then load the models.
  {
    const std::string model_file_name("D:/game_assets/models/ruby-rose/source/rubyAnimated002.fbx");
    const std::string model_name("ruby");
    std::vector<std::unique_ptr<assets::Assets>> assets;

    LoadSingleModel(model_file_name, model_name);
    auto mesh_vertices = std::make_unique<assets::MeshVertices>();
    models_[model_name].GetAllMeshVertexData(mesh_vertices->model_all_meshes_vertices_data_, mesh_vertices->model_all_meshes_vertices_size_, mesh_vertices->model_all_meshes_vertices_number_);
    assets.emplace_back(std::move(mesh_vertices));
    
    if (renderer) {
      renderer->LoadAssets(assets);
    }
  }

}

void Scene::LoadCamera() {
  // Note(wushiyuan): For a real game engine: read config file to determine camera's position and orientation.
  Eigen::Vector3f camera_pos(0.f, 2.f, 2.f);
  Eigen::Vector3f look_target(0.f, 1.f, 0.f);
  Eigen::Vector3f up(0.f, 1.f, 0.f);
  camera_.Set(camera_pos, look_target, up);
}

void Scene::LoadLights() {
}

void Scene::LoadSingleModel(const std::string& model_file_name, const std::string& model_name) {
  if (ModelImporter::GetSharedInstance().LoadModelFile(model_file_name)) {
    models_.emplace(model_name, Model(model_name));
    models_[model_name].LoadMeshes();
  }
}

}  // namespace motion_animation