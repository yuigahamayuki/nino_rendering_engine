#include "scene.h"

#include "model_importer.h"

namespace motion_animation {

void Scene::LoadSceneAssets() {
  LoadModels();
  LoadCamera();
  LoadLights();
}

void Scene::LoadModels() {
  // For a real game engine: read config file to determine which models are contained in the scene, then load the models.
  {
    const std::string model_file_name("D:/game_assets/models/ruby-rose/source/rubyAnimated002.fbx");
    const std::string model_name("ruby");
    LoadSingleModel(model_file_name, model_name);
    std::vector<Mesh::Vertex> model_all_meshes_vertices_data;
    size_t model_all_meshes_vertices_size = 0;
    size_t model_all_meshes_vertices_number = 0;
    models_[model_name].GetAllMeshVertexData(model_all_meshes_vertices_data, model_all_meshes_vertices_size, model_all_meshes_vertices_number);
    
    // TODO(wushiyuan): load vertices to gpu
  }

}

void Scene::LoadCamera() {
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