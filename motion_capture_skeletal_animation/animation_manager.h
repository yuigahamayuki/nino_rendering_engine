#pragma once

#include <map>
#include <string>

namespace motion_animation {

class AnimationManager {
 public:
   typedef std::map<std::string, uint32_t> BonesMap;  // key: bone_name value: bone_index

  static AnimationManager& GetSharedInstance();

  ~AnimationManager() = default;

  AnimationManager(const AnimationManager&) = delete;
  AnimationManager& operator=(const AnimationManager&) = delete;

  void MakeBonesMapForModel(const std::string& model_name);
  uint32_t FindBoneIndexForNameOfModel(const std::string& model_name, const std::string& bone_name);
 private:

  AnimationManager() = default;

  std::map<std::string, BonesMap> bones_of_all_models_;  // key: model_name value: BonesMap for that model
};  // class AnimationManager

}  // namespace motion_animation