#pragma once

#include <set>

#include "mesh.h"

namespace motion_animation {

namespace assets {

enum class AssetType {
  unknown,
  vertex,
  texture
};  // enum class AssetType

struct Asset {
  Asset() = default;
  virtual ~Asset() = default;

  AssetType asset_type() const {
    return asset_type_;
  }

 protected:
  AssetType asset_type_ = AssetType::unknown;
};  // class Assets

struct MeshVertices : public Asset {
  MeshVertices() {
    asset_type_ = AssetType::vertex;
  }

  std::vector<Mesh::Vertex> model_all_meshes_vertices_data_;
  size_t model_all_meshes_vertices_size_ = 0;
  size_t model_all_meshes_vertices_number_ = 0;

  std::vector<uint32_t> model_all_meshes_indices_data_;
  size_t model_all_meshes_indices_size_ = 0;
  size_t model_all_meshes_indices_number_ = 0;
};  // struct MeshVertices

struct Textures : public Asset {
  Textures() {
    asset_type_ = AssetType::texture;
  }

  // Used to compile shader using macro for different texture type combinations.
  // For example, in fragment shader the code is selectively compiled to
  // use normal map depending on whether the model has normal maps.
  std::set<Mesh::Texture::TextureType> texture_type_set_;
  std::vector<std::string> textures_file_paths_;
};

}  // namespace assets

}  // namespace motion_animation