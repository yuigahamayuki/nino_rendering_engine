#pragma once

#include <string>
#include <vector>
#include <memory>

#include "mesh.h"

namespace motion_animation {

class Model {
 public:
  Model() = default;
  Model(const std::string& model_name) : model_name_(model_name) {

  }
  ~Model() = default;

  Model(const Model&) = delete;
  Model& operator=(const Model&) = delete;

  Model(Model&&) = default;
  Model& operator=(Model&&) = default;

  const size_t GetTotalVerticesNumberForAllMeshes() const {
    return total_vertices_number_for_all_meshes_;
  }

  void SetTotalVerticesNumberForAllMeshes(size_t total_vertices_number_for_all_meshes) {
    total_vertices_number_for_all_meshes_ = total_vertices_number_for_all_meshes;
  }

  void LoadMeshes();

  void GetAllMeshVertexData(std::vector<Mesh::Vertex>& all_meshes_vertices_data, size_t& all_meshes_vertices_size, size_t& all_meshes_vertices_number);

  void LoadAnimations();

 private:
  std::string model_name_;

  std::vector<std::unique_ptr<Mesh>> meshes_;

  size_t total_vertices_number_for_all_meshes_ = 0;
};  // class Model

}  // namespace motion_animation