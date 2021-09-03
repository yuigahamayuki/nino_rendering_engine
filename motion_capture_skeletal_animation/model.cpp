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

void Model::LoadAnimations() {
  ModelImporter::GetSharedInstance().LoadAllAnimationsForModel(model_name_);
}

}  // namespace motion_animation