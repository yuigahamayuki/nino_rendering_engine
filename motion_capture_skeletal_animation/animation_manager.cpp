#include "animation_manager.h"

namespace motion_animation {

AnimationManager& AnimationManager::GetSharedInstance() {
  static AnimationManager animation_manager;
  return animation_manager;
}

void AnimationManager::MakeBonesMapForModel(const std::string& model_name) {
  bones_of_all_models_.emplace(model_name, BonesMap());
}

void AnimationManager::MakeBonesInfoForModel(const std::string& model_name) {
  bones_info_of_all_models_.emplace(model_name, std::vector<AnimationBone>());
}

uint32_t AnimationManager::FindBoneIndexForModel(const std::string& model_name, const std::string& bone_name, bool& bone_is_exist) {
  uint32_t bone_index = 0;
  if (bones_of_all_models_.find(model_name) == bones_of_all_models_.end()) {
    return bone_index;
  }

  if (bones_of_all_models_[model_name].find(bone_name) != bones_of_all_models_[model_name].end()) {
    bone_index = bones_of_all_models_[model_name][bone_name];
    bone_is_exist = true;
  } else {
    auto total_bone_number = static_cast<uint32_t>(bones_of_all_models_[model_name].size());
    bones_of_all_models_[model_name].emplace(bone_name, total_bone_number);
    bone_index = total_bone_number;
    bone_is_exist = false;
  }

  return bone_index;
}

void AnimationManager::InsertBoneInfoForModel(const std::string& model_name, const AnimationBone& bone_info) {
  if (bones_info_of_all_models_.find(model_name) == bones_info_of_all_models_.end()) {
    return;
  }

  bones_info_of_all_models_[model_name].emplace_back(bone_info);
}

void AnimationManager::InsertAnimationForModel(const std::string& model_name, const std::string& animation_name, const Animation & animation) {
  animations_of_all_models_.insert({ model_name, AnimationsMap() });
  animations_of_all_models_[model_name].insert({ animation_name, animation });
}

void AnimationManager::InsertAnimationRootNodeForModel(const std::string& model_name, std::shared_ptr<AnimationNode> animation_node) {
  animation_nodes_of_all_models_.insert({ model_name, animation_node });
}

}  // namespace motion_animation
