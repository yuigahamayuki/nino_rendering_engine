#include "scene.h"

#include "model_importer.h"
#include "assets.h"

namespace motion_animation {

void Scene::LoadSceneAssets(Renderer* renderer) {
  LoadModels(renderer);
  LoadCamera();
  LoadLights();
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
  // Note(wushiyuan): For a real game engine: read config file to determine which models are contained in the scene, then load the models.
  {
    // const std::string model_file_name("D:/game_assets/models/ruby-rose/source/rubyAnimated002.fbx");
    // const std::string model_file_name("D:/game_assets/models/blender_test_animation/blender_test_animation_rotate.fbx");
    const std::string model_file_name("D:/game_assets/models/blender_test_animation/ruby_processed.fbx");
    const std::string model_name("ruby");
    std::vector<std::unique_ptr<assets::Asset>> assets;

    LoadSingleModel(model_file_name, model_name);
    auto mesh_vertices = std::make_unique<assets::MeshVertices>();
    models_.back().GetAllMeshesVertexAndIndexData(mesh_vertices->model_all_meshes_vertices_data_, mesh_vertices->model_all_meshes_vertices_size_, mesh_vertices->model_all_meshes_vertices_number_,
      mesh_vertices->model_all_meshes_indices_data_, mesh_vertices->model_all_meshes_indices_size_, mesh_vertices->model_all_meshes_indices_number_);
    models_.back().SetTotalVerticesNumberForAllMeshes(mesh_vertices->model_all_meshes_vertices_number_);
    models_.back().SetTotalIndicesNumberForAllMeshes(mesh_vertices->model_all_meshes_indices_number_);
    assets.emplace_back(std::move(mesh_vertices));
    
    if (renderer) {
      renderer->LoadAssets(assets);
    }
  }

}

void Scene::LoadCamera() {
  // Note(wushiyuan): For a real game engine: read config file to determine camera's position and orientation.
  Eigen::Vector3f camera_pos(0.f, 2.f, 2.f);
  Eigen::Vector3f look_target(0.f, 1.f, 0.f);
  Eigen::Vector3f up(0.f, 1.f, 0.f);
  camera_.Set(camera_pos, look_target, up);
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