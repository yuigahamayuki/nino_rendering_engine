#pragma once

#include "mesh.h"

#include <assimp/scene.h>

namespace motion_animation {

class AssimpMesh : public Mesh {
 public:
  AssimpMesh(const aiScene* assimp_scene_ptr, uint32_t mesh_index_of_assimp);
  ~AssimpMesh() = default;

  void GetVertexData(std::vector<Vertex>& vertices_data, size_t& vertices_size, size_t& vertices_number) override;

 private:
  const aiScene* assimp_scene_ptr_ = nullptr;
  uint32_t mesh_index_of_assimp_ = 0;
};  // class AssimpMesh

}  // namespace motion_animation