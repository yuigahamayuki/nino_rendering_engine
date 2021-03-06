#pragma once

#include "mesh.h"

#include <assimp/scene.h>

namespace motion_animation {

class AssimpMesh : public Mesh {
 public:
  AssimpMesh(const aiScene* assimp_scene_ptr, uint32_t mesh_index_of_assimp, const std::string& model_name);
  ~AssimpMesh() = default;

  // Note(wushiyuan): AssimpMesh does not own assimp_scene_ptr_,
  // so if ModelImporter::LoadModelFile is called to load another model before calling AssimpMesh::GetVertexData,
  // then AssimpMesh::GetVertexData should not be called because AssimpMesh::assimp_scene_ptr_ becomes invalid
  void GetVertexData(std::vector<Vertex>& vertices_data, size_t& vertices_size, size_t& vertices_number) override;

  void GetIndexData(std::vector<uint32_t>& indices_data, size_t& indices_size, size_t& indices_number) override;

  void GetTexturesTypesAndFilePaths(std::set<Texture::TextureType>& texture_type_set, std::vector<std::string>& textures_file_paths) override;

  void GetBlendshapeData(std::vector<BlendshapeVertex>& blendshape_vertices_data, size_t& blendshape_vertices_size, size_t& blendshape_vertices_number) override;

 private:
   void GetTexturesFilePathsForTextureType(const aiMaterial* material, aiTextureType assimp_texture_type, std::set<Texture::TextureType>& texture_type_set, std::vector<std::string>& textures_file_paths);

  const aiScene* assimp_scene_ptr_ = nullptr;
  uint32_t mesh_index_of_assimp_ = 0;
};  // class AssimpMesh

}  // namespace motion_animation