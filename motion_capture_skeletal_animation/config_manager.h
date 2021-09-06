#pragma once

#include <string>
#include <vector>

#include <rapidjson/document.h>

namespace motion_animation {

namespace config {

struct SceneModelConfig {
  std::string model_name_;
  std::string model_path_;
};

struct SceneCameraConfig {
  float pos_x = 0.0f;
  float pos_y = 0.0f;
  float pos_z = 0.0f;
  float look_x = 0.0f;
  float look_y = 0.0f;
  float look_z = 0.0f;
  float up_x = 0.0f;
  float up_y = 0.0f;
  float up_z = 0.0f;
};

}  // namespace config

class ConfigManager {
public:
  static ConfigManager& GetSharedInstance();

  ~ConfigManager() = default;

  ConfigManager(const ConfigManager&) = delete;
  ConfigManager& operator=(const ConfigManager&) = delete;

  void ParseScenesConfigFile();

  void GetModelsConfigOfScene(const std::string& scene_name, std::vector<config::SceneModelConfig>& scene_models_config) const;

  void GetCameraConfigOfScene(const std::string& scene_name, config::SceneCameraConfig& scene_camera_config) const;

private:
  static const std::string kScenesConfigFilePath;

  ConfigManager() = default;

  rapidjson::Document document_;
};  // class ConfigManager

}  // namespace motion_animation