#include "animation_manager.h"

namespace motion_animation {

AnimationManager& AnimationManager::GetSharedInstance() {
  static AnimationManager animation_manager;
  return animation_manager;
}

void AnimationManager::MakeBonesMapForModel(const std::string& model_name) {
  bones_of_all_models_.emplace(model_name, BonesMap());
}

uint32_t AnimationManager::FindBoneIndexForNameOfModel(const std::string& model_name, const std::string& bone_name) {
  uint32_t bone_index = 0;
  if (bones_of_all_models_.find(model_name) == bones_of_all_models_.end()) {
    return bone_index;
  }

  if (bones_of_all_models_[model_name].find(bone_name) != bones_of_all_models_[model_name].end()) {
    bone_index = bones_of_all_models_[model_name][bone_name];
  } else {
    auto total_bone_number = static_cast<uint32_t>(bones_of_all_models_[model_name].size());
    bones_of_all_models_[model_name].emplace(bone_name, total_bone_number);
    bone_index = total_bone_number;
  }

  return bone_index;
}

}  // namespace motion_animation
