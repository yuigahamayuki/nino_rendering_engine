#include "model_importer.h"

#include <iostream>

#include <assimp/postprocess.h>

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
  }

  return load_success;
}

void ModelImporter::LoadAllMeshesForModel(const std::string& model_name, std::vector<std::unique_ptr<Mesh>>& meshes) {
  if (assimp_scene_ptr_) {
    AnimationManager::GetSharedInstance().MakeBonesMapForModel(model_name);
    auto total_mesh_number = assimp_scene_ptr_->mNumMeshes;
    for (uint32_t i = 0; i < total_mesh_number; ++i) {
      meshes.emplace_back(std::make_unique<AssimpMesh>(assimp_scene_ptr_, i));
    }
  }
}

void ModelImporter::LoadAllAnimationsForModel(const std::string& model_name) {
  if (assimp_scene_ptr_) {
    auto animation_number = assimp_scene_ptr_->mNumAnimations;
    for (uint32_t i = 0; i < animation_number; ++i) {
      std::string animation_name(assimp_scene_ptr_->mAnimations[i]->mName.C_Str());
      double duration_in_ticks = assimp_scene_ptr_->mAnimations[i]->mDuration;
      double ticks_per_second = assimp_scene_ptr_->mAnimations[i]->mTicksPerSecond;

      Animation animation(animation_name, duration_in_ticks, ticks_per_second);
      for (uint32_t j = 0; j < assimp_scene_ptr_->mAnimations[i]->mNumChannels; ++j) {
        animation.InsertOneChannelFromAssimp(assimp_scene_ptr_->mAnimations[i]->mChannels[j]->mNodeName.C_Str(),
          assimp_scene_ptr_->mAnimations[i]->mChannels[j]->mNumScalingKeys,
          assimp_scene_ptr_->mAnimations[i]->mChannels[j]->mScalingKeys,
          assimp_scene_ptr_->mAnimations[i]->mChannels[j]->mNumRotationKeys,
          assimp_scene_ptr_->mAnimations[i]->mChannels[j]->mRotationKeys,
          assimp_scene_ptr_->mAnimations[i]->mChannels[j]->mNumPositionKeys,
          assimp_scene_ptr_->mAnimations[i]->mChannels[j]->mPositionKeys);
      }
     

      AnimationManager::GetSharedInstance().InsertAnimationForModel(model_name, animation_name, animation);
    }
  }
}

}  // namespace motion_animation