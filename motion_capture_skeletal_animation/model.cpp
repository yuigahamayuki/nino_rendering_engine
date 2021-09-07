#include "model.h"

#include "model_importer.h"

namespace motion_animation {

void Model::LoadMeshes() {
  ModelImporter::GetSharedInstance().LoadAllMeshesForModel(model_name_, meshes_);
}

void Model::GetAllMeshesVertexAndIndexData(std::vector<Mesh::Vertex>& all_meshes_vertices_data, size_t& all_meshes_vertices_size, size_t& all_meshes_vertices_number,
  std::vector<uint32_t>& all_meshes_indices_data, size_t& all_meshes_indices_size, size_t& all_meshes_indices_number) {
  std::for_each(meshes_.begin(), meshes_.end(), [&all_meshes_vertices_data, &all_meshes_vertices_size, &all_meshes_vertices_number, &all_meshes_indices_data, &all_meshes_indices_size, &all_meshes_indices_number](std::unique_ptr<Mesh> &mesh) {
    // Load vertices
    std::vector<Mesh::Vertex> single_mesh_vertices_data;
    size_t single_mesh_vertices_size = 0;
    size_t single_mesh_vertices_number = 0;
    mesh->GetVertexData(single_mesh_vertices_data, single_mesh_vertices_size, single_mesh_vertices_number);

    all_meshes_vertices_data.insert(all_meshes_vertices_data.end(), single_mesh_vertices_data.begin(), single_mesh_vertices_data.end());
    all_meshes_vertices_size += single_mesh_vertices_size;
    all_meshes_vertices_number += single_mesh_vertices_number;


    // Load indices
    std::vector<uint32_t> single_mesh_indices_data;
    size_t single_mesh_indices_size = 0;
    size_t single_mesh_indices_number = 0;
    mesh->GetIndexData(single_mesh_indices_data, single_mesh_indices_size, single_mesh_indices_number);

    all_meshes_indices_data.insert(all_meshes_indices_data.end(), single_mesh_indices_data.begin(), single_mesh_indices_data.end());
    all_meshes_indices_size += single_mesh_indices_size;
    all_meshes_indices_number += single_mesh_indices_number;
  });
}

void Model::GetAllMeshesTexturesFilePaths(std::vector<std::string>& all_meshes_textures_file_paths) {
  std::for_each(meshes_.begin(), meshes_.end(), [&all_meshes_textures_file_paths](std::unique_ptr<Mesh>& mesh) {
    std::vector<std::string> single_mesh_textures_file_paths;
    mesh->GetTexturesFilePaths(single_mesh_textures_file_paths);

    all_meshes_textures_file_paths.insert(all_meshes_textures_file_paths.end(), single_mesh_textures_file_paths.cbegin(), single_mesh_textures_file_paths.cend());
  });
}

void Model::LoadAnimations() {
  ModelImporter::GetSharedInstance().LoadAllAnimationsForModel(model_name_);
}

void Model::GetAllMeshesDrawArguments(std::vector<Mesh::DrawArugument>& draw_arguments) const {
  if (meshes_.empty()) {
    return;
  }

  draw_arguments.clear();
  draw_arguments.resize(meshes_.size());

  draw_arguments[0].index_count_ = static_cast<uint32_t>(meshes_[0]->GetIndicesNumber());
  draw_arguments[0].vertex_count_ = static_cast<uint32_t>(meshes_[0]->GetVerticesNumber());
  draw_arguments[0].index_start_ = 0;
  draw_arguments[0].vertex_base_ = 0;

  uint32_t accumulated_index_start = static_cast<uint32_t>(meshes_[0]->GetIndicesNumber());
  uint32_t accumulated_vertex_base = static_cast<uint32_t>(meshes_[0]->GetVerticesNumber());

  for (size_t i = 1; i < meshes_.size(); ++i) {
    draw_arguments[i].index_count_ = static_cast<uint32_t>(meshes_[i]->GetIndicesNumber());
    draw_arguments[i].vertex_count_ = static_cast<uint32_t>(meshes_[i]->GetVerticesNumber());
    draw_arguments[i].index_start_ = accumulated_index_start;
    draw_arguments[i].vertex_base_ = accumulated_vertex_base;

    accumulated_index_start += static_cast<uint32_t>(meshes_[i]->GetIndicesNumber());
    accumulated_vertex_base += static_cast<uint32_t>(meshes_[i]->GetVerticesNumber());
  }
}

}  // namespace motion_animation