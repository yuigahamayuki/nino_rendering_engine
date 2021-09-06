#include "animation_player.h"

#include "util_linear_algebra_helper.h"

namespace motion_animation {

AnimationPlayer& AnimationPlayer::GetSharedInstance() {
  static AnimationPlayer animation_player;
  return animation_player;
}

void AnimationPlayer::ComputeBoneTransforms(double time_in_seconds, std::vector<Eigen::Matrix4f>& bone_transforms, const AnimationManager::BonesMap& bones_map, 
  std::vector<AnimationBone>& bones_info, const Animation& animation, const std::shared_ptr<AnimationNode> node) {
  double animation_duration_in_ticks = animation.duration_in_ticks();
  double animation_ticks_per_second = animation.ticks_per_second();
  double time_in_ticks = time_in_seconds * animation_ticks_per_second;

  Eigen::Matrix4f identity(Eigen::Matrix4f::Identity());
  bone_transforms.resize(bones_info.size());
  ReadAnimationNodeHeirarchy(time_in_ticks, bone_transforms, bones_map, bones_info, animation, node, identity);
}

void AnimationPlayer::ReadAnimationNodeHeirarchy(double time_in_ticks, std::vector<Eigen::Matrix4f>& bone_transforms, const AnimationManager::BonesMap& bones_map, 
  std::vector<AnimationBone>& bones_info, const Animation& animation, const std::shared_ptr<AnimationNode> node, const Eigen::Matrix4f& parent_transform) {
  const std::string current_process_node_name = node->node_name();
  Eigen::Matrix4f current_process_node_local_transform = node->transform();

  const Animation::Channel* channel_ptr = animation.GetChannelForName(current_process_node_name);
  if (channel_ptr) {
    const Eigen::Matrix4f scaling = CalculateInterpolatedScaling(time_in_ticks, channel_ptr);
    const Eigen::Matrix4f rotation = CalculateInterpolatedRotation(time_in_ticks, channel_ptr);
    const Eigen::Matrix4f translation = CalculateInterpolatedTranslation(time_in_ticks, channel_ptr);

    current_process_node_local_transform = translation * rotation * scaling;
  }

  Eigen::Matrix4f current_process_node_global_transform = parent_transform * current_process_node_local_transform;

  if (bones_map.find(current_process_node_name) != bones_map.end()) {
    uint32_t bone_index = bones_map.at(current_process_node_name);
    // Note(wushiyuan): is global_inv necessary ?
    const Eigen::Matrix4f final_transform = animation.global_inverse_transform() * current_process_node_global_transform * bones_info[bone_index].offset_matrix();
    bone_transforms[bone_index] = final_transform;
    bones_info[bone_index].set_final_transform_matrix(final_transform);
  }

  for (auto child_node : node->children_) {
    ReadAnimationNodeHeirarchy(time_in_ticks, bone_transforms, bones_map, bones_info, animation, child_node, current_process_node_global_transform);
  }
}

Eigen::Matrix4f AnimationPlayer::CalculateInterpolatedScaling(double time_in_ticks, const Animation::Channel* channel) {
  int key_frame_index = GetScalingKeyFrameIndexOfTime(time_in_ticks, channel);

  const Eigen::Vector3f start(channel->scaling_keys_[key_frame_index].scale_x_,
    channel->scaling_keys_[key_frame_index].scale_y_,
    channel->scaling_keys_[key_frame_index].scale_z_);
  const Eigen::Vector3f end(channel->scaling_keys_[key_frame_index + 1].scale_x_,
    channel->scaling_keys_[key_frame_index + 1].scale_y_,
    channel->scaling_keys_[key_frame_index + 1].scale_z_);

  double start_time = channel->scaling_keys_[key_frame_index].key_frame_time_in_ticks_;
  double end_time = channel->scaling_keys_[key_frame_index + 1].key_frame_time_in_ticks_;
  float t = static_cast<float>((time_in_ticks - start_time) / (end_time - start_time));

  const Eigen::Vector3f interpolated = util::Lerp(start, end, t);

  return Eigen::Affine3f(Eigen::AlignedScaling3f(interpolated)).matrix();
}

Eigen::Matrix4f AnimationPlayer::CalculateInterpolatedRotation(double time_in_ticks, const Animation::Channel* channel) {
  int key_frame_index = GetRotationKeyFrameIndexOfTime(time_in_ticks, channel);

  const Eigen::Quaternionf start(channel->rotation_keys_[key_frame_index].quaternion_w_,
    channel->rotation_keys_[key_frame_index].quaternion_x_,
    channel->rotation_keys_[key_frame_index].quaternion_y_,
    channel->rotation_keys_[key_frame_index].quaternion_z_);
  const Eigen::Quaternionf end(channel->rotation_keys_[key_frame_index + 1].quaternion_w_,
    channel->rotation_keys_[key_frame_index + 1].quaternion_x_,
    channel->rotation_keys_[key_frame_index + 1].quaternion_y_,
    channel->rotation_keys_[key_frame_index + 1].quaternion_z_);

  double start_time = channel->rotation_keys_[key_frame_index].key_frame_time_in_ticks_;
  double end_time = channel->rotation_keys_[key_frame_index + 1].key_frame_time_in_ticks_;
  float t = static_cast<float>((time_in_ticks - start_time) / (end_time - start_time));

  Eigen::Quaternionf interpolated = start.slerp(t, end);

  Eigen::Matrix4f rotation_matrix = Eigen::Matrix4f::Identity();
  rotation_matrix.block(0, 0, 3, 3) = interpolated.normalized().toRotationMatrix();
  return rotation_matrix;
}

Eigen::Matrix4f AnimationPlayer::CalculateInterpolatedTranslation(double time_in_ticks, const Animation::Channel* channel) {
  int key_frame_index = GetTranslationKeyFrameIndexOfTime(time_in_ticks, channel);

  const Eigen::Vector3f start(channel->translation_keys_[key_frame_index].translation_x_,
    channel->translation_keys_[key_frame_index].translation_y_,
    channel->translation_keys_[key_frame_index].translation_z_);
  const Eigen::Vector3f end(channel->translation_keys_[key_frame_index + 1].translation_x_,
    channel->translation_keys_[key_frame_index + 1].translation_y_,
    channel->translation_keys_[key_frame_index + 1].translation_z_);

  double start_time = channel->translation_keys_[key_frame_index].key_frame_time_in_ticks_;
  double end_time = channel->translation_keys_[key_frame_index + 1].key_frame_time_in_ticks_;
  float t = static_cast<float>((time_in_ticks - start_time) / (end_time - start_time));

  const Eigen::Vector3f interpolated = util::Lerp(start, end, t);

  return Eigen::Affine3f(Eigen::Translation3f(interpolated)).matrix();
}

int AnimationPlayer::GetScalingKeyFrameIndexOfTime(double time_in_ticks, const Animation::Channel* channel) {
  int key_frame_index = -1;
  for (int i = 0; i < channel->scaling_keys_.size() - 1; ++i) {
    if (time_in_ticks < channel->scaling_keys_[i + 1].key_frame_time_in_ticks_) {
      key_frame_index = i;
      break;
    }
  }

  return key_frame_index;
}

int AnimationPlayer::GetRotationKeyFrameIndexOfTime(double time_in_ticks, const Animation::Channel* channel) {
  int key_frame_index = -1;
  for (int i = 0; i < channel->rotation_keys_.size() - 1; ++i) {
    if (time_in_ticks < channel->rotation_keys_[i + 1].key_frame_time_in_ticks_) {
      key_frame_index = i;
      break;
    }
  }

  return key_frame_index;
}

int AnimationPlayer::GetTranslationKeyFrameIndexOfTime(double time_in_ticks, const Animation::Channel* channel) {
  int key_frame_index = -1;
  for (int i = 0; i < channel->translation_keys_.size() - 1; ++i) {
    if (time_in_ticks < channel->translation_keys_[i + 1].key_frame_time_in_ticks_) {
      key_frame_index = i;
      break;
    }
  }

  return key_frame_index;
}

}  // namespace motion_animation