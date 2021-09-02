#include "animation.h"

namespace motion_animation {
  
void Animation::InsertOneChannelFromAssimp(const std::string& node_name, int scaling_keys_number, void* scaling_keys, int rotation_keys_number, void* rotation_keys, 
  int translation_keys_number, void* translation_keys) {
  Channel channel;
  channel.node_name_ = node_name;

  channel.scaling_keys_.resize(scaling_keys_number);
  std::memcpy(channel.scaling_keys_.data(), scaling_keys, sizeof(ScalingKey) * scaling_keys_number);

  channel.rotation_keys_.resize(rotation_keys_number);
  std::memcpy(channel.rotation_keys_.data(), rotation_keys, sizeof(RotationKey) * rotation_keys_number);

  channel.translation_keys_.resize(translation_keys_number);
  std::memcpy(channel.translation_keys_.data(), translation_keys, sizeof(ScalingKey) * translation_keys_number);

  channels_.emplace_back(channel);
}

}  // namespace motion_animation