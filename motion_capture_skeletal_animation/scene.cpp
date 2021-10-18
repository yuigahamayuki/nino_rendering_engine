#include "scene.h"

#include "model_importer.h"
#include "assets.h"
#include "config_manager.h"
#include "renderer.h"

namespace motion_animation {

void Scene::LoadSceneAssets(Renderer* renderer) {
  LoadModels(renderer);
  LoadCamera();
  LoadLights();
}

void Scene::Update() {
  camera_.Update();
}

void Scene::GetAllModelsDrawArguments(std::vector<Mesh::DrawArugument>& all_models_draw_arguments) const {
  all_models_draw_arguments.clear();

  int model_index = 0;
  uint32_t accumulated_vertex_offset = 0;
  uint32_t accumulated_index_offset = 0;
  // Note(wushiyuan): carefully check the implementation.
  std::for_each(models_.cbegin(), models_.cend(), [&all_models_draw_arguments, &model_index, &accumulated_vertex_offset, &accumulated_index_offset](const Model& model) {
    std::vector<Mesh::DrawArugument> single_model_draw_arguments;
    model.GetAllMeshesDrawArguments(single_model_draw_arguments);
    
    // TODO(wushiyuan): multi model blendshape draw arguments.
    uint32_t single_model_total_vertices_number = single_model_draw_arguments.back().vertex_base_ + single_model_draw_arguments.back().vertex_count_;
    uint32_t single_model_total_indices_number = single_model_draw_arguments.back().index_start_ + single_model_draw_arguments.back().index_count_;

    if (model_index > 0) {
      std::for_each(single_model_draw_arguments.begin(), single_model_draw_arguments.end(), [accumulated_vertex_offset, accumulated_index_offset](Mesh::DrawArugument& single_mesh_draw_argument) {
        single_mesh_draw_argument.index_start_ += accumulated_index_offset;
        single_mesh_draw_argument.vertex_base_ += accumulated_vertex_offset;
      });
    }

    accumulated_vertex_offset += single_model_total_vertices_number;
    accumulated_index_offset += single_model_total_indices_number;
    model_index++;

    all_models_draw_arguments.insert(all_models_draw_arguments.end(), single_model_draw_arguments.cbegin(), single_model_draw_arguments.cend());
  });
}

void Scene::LoadModels(Renderer* renderer) {
  std::vector<std::unique_ptr<assets::Asset>> assets;
  std::vector<config::SceneModelConfig> scene_models_config;
  ConfigManager::GetSharedInstance().GetModelsConfigOfScene(scene_name_, scene_models_config);

  auto mesh_vertices_asset = std::make_unique<assets::MeshVertices>();
  auto textures_asset = std::make_unique<assets::Textures>();

  for (size_t i = 0; i < scene_models_config.size(); ++i) {
    LoadSingleModel(scene_models_config[i].model_path_, scene_models_config[i].model_name_);
    // Prepare vertices to upload
    models_.back().GetAllMeshesVertexAndIndexData(mesh_vertices_asset->model_all_meshes_vertices_data_, mesh_vertices_asset->model_all_meshes_vertices_size_, mesh_vertices_asset->model_all_meshes_vertices_number_,
      mesh_vertices_asset->model_all_meshes_indices_data_, mesh_vertices_asset->model_all_meshes_indices_size_, mesh_vertices_asset->model_all_meshes_indices_number_);
    models_.back().SetTotalVerticesNumberForAllMeshes(mesh_vertices_asset->model_all_meshes_vertices_number_);
    models_.back().SetTotalIndicesNumberForAllMeshes(mesh_vertices_asset->model_all_meshes_indices_number_);   

    // Prepare textures file paths to read and upload  
    models_.back().GetAllMeshesTexturesFilePaths(textures_asset->texture_type_set_, textures_asset->textures_file_paths_);

    // Prepare blendshape data to upload
    models_.back().GetAllMeshesBlendshapeData(mesh_vertices_asset->model_all_meshes_blendshape_vertices_data_, mesh_vertices_asset->model_all_blendshape_vertices_size_, 
      mesh_vertices_asset->model_all_blendshape_vertices_number_, mesh_vertices_asset->blendshape_number_);
  }

  assets.emplace_back(std::move(mesh_vertices_asset));
  assets.emplace_back(std::move(textures_asset));

  if (renderer) {
    renderer->LoadAssets(assets);
  }
}

void Scene::LoadCamera() {
  config::SceneCameraConfig scene_camera_config;
  ConfigManager::GetSharedInstance().GetCameraConfigOfScene(scene_name_, scene_camera_config);

  Eigen::Vector3f camera_pos(scene_camera_config.pos_x, scene_camera_config.pos_y, scene_camera_config.pos_z);
  Eigen::Vector3f look_direction(scene_camera_config.look_x, scene_camera_config.look_y, scene_camera_config.look_z);
  Eigen::Vector3f up(scene_camera_config.up_x, scene_camera_config.up_y, scene_camera_config.up_z);
  camera_.Set(camera_pos, look_direction, up);
}

void Scene::LoadLights() {
}

void Scene::LoadSingleModel(const std::string& model_file_name, const std::string& model_name) {
  if (ModelImporter::GetSharedInstance().LoadModelFile(model_file_name)) {
    models_.emplace_back(Model(model_name));
    models_.back().LoadMeshes();
    models_.back().LoadAnimations();
  }
}

}  // namespace motion_animation