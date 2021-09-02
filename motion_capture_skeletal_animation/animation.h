#pragma once

#include <string>
#include <vector>

#include <Eigen/Eigen>

namespace motion_animation {

// Note(wushiyuan): assimp node, including bones for animation
class AnimationNode {
public:

private:

};

class AnimationBone {
public:

private:
  std::string bone_name_;
  // Convert from model space to this bone's local space defined in bind pose.
  // Once set, will not be updated anymore.
  Eigen::Matrix4f offset_matrix_;
  // Convert from model space to model space.
  // Updated every frame.
  // Multiply this matrix with the original model space vertex (in bind pose),
  // the position of the animated vertex canbe computed.
  Eigen::Matrix4f final_transform_matrix_;
};

class Animation {
public:
  Animation(const std::string& animation_name, double duration_in_ticks, double ticks_per_second) 
    : animation_name_(animation_name), duration_in_ticks_(duration_in_ticks), ticks_per_second_(ticks_per_second) {

  }

  // void* is the array pointer, directly using memcpy to copy the members, so the definition of Key should be the same as assimp's.
  void InsertOneChannelFromAssimp(const std::string& node_name, int scaling_keys_number, void* scaling_keys, int rotation_keys_number, void* rotation_keys, 
    int translation_keys_number, void* translation_keys);

private:
  struct ScalingKey {
    double key_frame_time_in_ticks_ = 0.0;
    float scale_x_ = 0.f;
    float scale_y_ = 0.f;
    float scale_z_ = 0.f;
  };
  struct RotationKey {
    double key_frame_time_in_ticks_ = 0.0;
    float quaternion_w_ = 0.f;
    float quaternion_x_ = 0.f;
    float quaternion_y_ = 0.f;
    float quaternion_z_ = 0.f;
  };
  struct TranslationKey {
    double key_frame_time_in_ticks_ = 0.0;
    float translation_x_ = 0.f;
    float translation_y_ = 0.f;
    float translation_z_ = 0.f;
  };
  struct Channel {
    std::string node_name_;
    std::vector<ScalingKey> scaling_keys_;
    std::vector<RotationKey> rotation_keys_;
    std::vector<TranslationKey> translation_keys_;
  };

  std::string animation_name_;

  double duration_in_ticks_ = 0.0;
  double ticks_per_second_ = 0.0;

  std::vector<Channel> channels_;
};


}  // namespace motion_animation