#include "config_manager.h"

#include <fstream>
#include <streambuf>

namespace motion_animation {

const std::string ConfigManager::kScenesConfigFilePath = "config/scenes.json";

ConfigManager& ConfigManager::GetSharedInstance() {
  static ConfigManager config_manager;
  return config_manager;
}

void ConfigManager::ParseScenesConfigFile() {
  std::ifstream t(ConfigManager::kScenesConfigFilePath);
  std::string json_str((std::istreambuf_iterator<char>(t)),
    std::istreambuf_iterator<char>());

  document_.Parse(json_str.c_str());
}

void ConfigManager::GetModelsConfigOfScene(const std::string& scene_name, std::vector<config::SceneModelConfig>& scene_models_config) const {
  scene_models_config.clear();

  const rapidjson::Value& scenes_value = document_["scenes"];
  for (rapidjson::SizeType i = 0; i < scenes_value.Size(); ++i) {
    const std::string json_scene_name = scenes_value[i]["scene name"].GetString();
    if (scene_name == json_scene_name) {
      const rapidjson::Value& models_value = scenes_value[i]["models"];

      for (rapidjson::SizeType j = 0; j < models_value.Size(); ++j) {
        config::SceneModelConfig scene_model_config;
        scene_model_config.model_name_ = models_value[j]["model name"].GetString();
        scene_model_config.model_path_ = models_value[j]["model path"].GetString();
        scene_models_config.emplace_back(scene_model_config);
      }

      break;
    }
  }
}

void ConfigManager::GetCameraConfigOfScene(const std::string& scene_name, config::SceneCameraConfig& scene_camera_config) const {
  const rapidjson::Value& scenes_value = document_["scenes"];
  for (rapidjson::SizeType i = 0; i < scenes_value.Size(); ++i) {
    const std::string json_scene_name = scenes_value[i]["scene name"].GetString();
    if (scene_name == json_scene_name) {
      const rapidjson::Value& json_camera = scenes_value[i]["camera"];
      scene_camera_config.pos_x = json_camera["pos x"].GetFloat();
      scene_camera_config.pos_y = json_camera["pos y"].GetFloat();
      scene_camera_config.pos_z = json_camera["pos z"].GetFloat();
      scene_camera_config.look_x = json_camera["look x"].GetFloat();
      scene_camera_config.look_y = json_camera["look y"].GetFloat();
      scene_camera_config.look_z = json_camera["look z"].GetFloat();
      scene_camera_config.up_x = json_camera["up x"].GetFloat();
      scene_camera_config.up_y = json_camera["up y"].GetFloat();
      scene_camera_config.up_z = json_camera["up z"].GetFloat();
      break;
    }
  }
}

}  // namespace motion_animation