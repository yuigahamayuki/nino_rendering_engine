#pragma once

#include <vector>

#include <Eigen/Eigen>

#include "animation_manager.h"

namespace motion_animation {

class AnimationPlayer {
public:
  static AnimationPlayer& GetSharedInstance();

  ~AnimationPlayer() = default;

  AnimationPlayer(const AnimationPlayer&) = delete;
  AnimationPlayer& operator=(const AnimationPlayer&) = delete;

  // void PlayAnimationForModel

  void ComputeBoneTransforms(double time_in_seconds, std::vector<Eigen::Matrix4f>& bone_transforms, const AnimationManager::BonesMap& bones_map,
    std::vector<AnimationBone>& bones_info, const Animation& animation, const std::shared_ptr<AnimationNode> node);

private:
  AnimationPlayer() = default;

  void ReadAnimationNodeHeirarchy(double time_in_ticks, std::vector<Eigen::Matrix4f>& bone_transforms, const AnimationManager::BonesMap& bones_map,
    std::vector<AnimationBone>& bones_info, const Animation& animation, const std::shared_ptr<AnimationNode> node, const Eigen::Matrix4f& parent_transform);

  Eigen::Matrix4f CalculateInterpolatedScaling(double time_in_ticks, const Animation::Channel* channel);
  Eigen::Matrix4f CalculateInterpolatedRotation(double time_in_ticks, const Animation::Channel* channel);
  Eigen::Matrix4f CalculateInterpolatedTranslation(double time_in_ticks, const Animation::Channel* channel);

  int GetScalingKeyFrameIndexOfTime(double time_in_ticks, const Animation::Channel* channel);
  int GetRotationKeyFrameIndexOfTime(double time_in_ticks, const Animation::Channel* channel);
  int GetTranslationKeyFrameIndexOfTime(double time_in_ticks, const Animation::Channel* channel);


};  // class AnimationPlayer

}  // namespace motion_animation