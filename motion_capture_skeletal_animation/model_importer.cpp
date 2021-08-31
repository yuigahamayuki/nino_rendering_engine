#include "model_importer.h"

#include <iostream>

#include <assimp/postprocess.h>

// TODO: test, delete
#include "assimp_mesh.h"
#include "animation_manager.h"

namespace motion_animation {

ModelImporter& ModelImporter::GetSharedInstance() {
  static ModelImporter model_importer;
  return model_importer;
}

ModelImporter::ModelImporter() {

}

bool ModelImporter::LoadModelFile(const std::string& model_file_path)
{
  bool load_success = false;
  assimp_scene_ptr_ = assimp_importer_.ReadFile(model_file_path.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals);
  if (assimp_scene_ptr_) {
    load_success = true;

    if (assimp_scene_ptr_->HasAnimations()) std::cout << "has animations" << std::endl;
    if (assimp_scene_ptr_->HasCameras()) std::cout << "has cameras" << std::endl;
    if (assimp_scene_ptr_->HasLights()) std::cout << "has lights" << std::endl;
    if (assimp_scene_ptr_->HasMaterials()) std::cout << "has materials" << std::endl;
    if (assimp_scene_ptr_->HasMeshes()) std::cout << "has meshes" << std::endl;
    if (assimp_scene_ptr_->HasTextures()) std::cout << "has textures" << std::endl;

    AnimationManager::GetSharedInstance().MakeBonesMapForModel("ruby");
    std::vector<Mesh::Vertex> vertices_data;
    size_t vertices_size = 0;
    size_t vertices_number = 0;
    AssimpMesh assimp_mesh(assimp_scene_ptr_, 0);
    assimp_mesh.GetVertexData(vertices_data, vertices_size, vertices_number);
  }

  return load_success;
}

}  // namespace motion_animation