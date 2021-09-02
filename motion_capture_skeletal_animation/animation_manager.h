#pragma once

#include <map>
#include <string>

#include "animation.h"

namespace motion_animation {

class AnimationManager {
 public:
   typedef std::map<std::string, uint32_t> BonesMap;  // key: bone_name value: bone_index
   typedef std::map<std::string, Animation> AnimationsMap;  // key: animation_name

  static AnimationManager& GetSharedInstance();

  ~AnimationManager() = default;

  AnimationManager(const AnimationManager&) = delete;
  AnimationManager& operator=(const AnimationManager&) = delete;

  void MakeBonesMapForModel(const std::string& model_name);
  uint32_t FindBoneIndexForNameOfModel(const std::string& model_name, const std::string& bone_name);

  void InsertAnimationForModel(const std::string& model_name, const std::string& animation_name, const Animation& animation);
 private:

  AnimationManager() = default;

  // Note(wushiyuan): may be the key should not be model_name, because skeleton and animation may be shared between multiple models.
  std::map<std::string, BonesMap> bones_of_all_models_;  // key: model_name value: BonesMap for that model
  std::map<std::string, AnimationsMap> animations_of_all_models_;  // key: model_name value: all the animations for the model of key
};  // class AnimationManager

}  // namespace motion_animation