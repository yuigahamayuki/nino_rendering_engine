#pragma once

#include <vector>
#include <string>
#include <cstdint>

namespace motion_animation {

class Mesh {
 public:
  struct Vertex {
    Vertex() = default;

    float positions_[3]{};
    float normals_[3]{};
    float texture_coordinates_[2]{};
    int bone_ids_[4]{-1, -1, -1, -1};
    float bone_weights_[4]{};
  };  // struct Vertex

  Mesh() = default;
  virtual ~Mesh() = default;

  const std::string GetMeshName() const {
    return mesh_name_;
  }

  const std::string GetDiffuseTextureName() const {
    return diffuse_texture_name_;
  }

  void SetMeshName(const std::string& mesh_name) {
    mesh_name_ = mesh_name;
  }

  void SetDiffuseTextureName(const std::string& diffuse_texture_name) {
    diffuse_texture_name_ = diffuse_texture_name;
  }

  virtual void GetVertexData(std::vector<Vertex>& vertices_data, size_t& vertices_size, size_t& vertices_number) = 0;
  //virtual void GetIndexData(std::vector<uint32_t>& indices_data, )

 private:
  std::string mesh_name_;
  std::string diffuse_texture_name_;

};  // class Mesh

}  // namespace motion_animation