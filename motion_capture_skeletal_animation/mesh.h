#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include <set>

namespace motion_animation {

class Mesh {
 public:
  struct Vertex {
    Vertex() = default;

    float position_[3]{};
    float normal_[3]{};
    float texture_coordinates_[2]{};
    int bone_ids_[4]{-1, -1, -1, -1};
    float bone_weights_[4]{};
  };  // struct Vertex

  struct Texture {
    enum class TextureType {
      diffuse,
      specular,
      normal,
      alpha,
    };

    Texture(TextureType texture_type, const std::string& texture_file_path) 
      : texture_type_(texture_type), texture_file_path_(texture_file_path) {

    }

    TextureType texture_type_ = TextureType::diffuse;
    std::string texture_file_path_;
  };

  struct BlendshapeVertex {
    BlendshapeVertex() = default;

    float position_[3]{};
    float normal_[3]{};
  };  // struct BlendshapeVertex

  struct DrawArugument {
    uint32_t index_count_ = 0;
    uint32_t index_start_ = 0;
    uint32_t vertex_count_ = 0;
    uint32_t vertex_base_ = 0;
    std::vector<Texture> textures_;
    uint32_t blendshape_number_ = 0;  // number of blendshapes for a mesh, e.g. 52 for ARKit-like blendshapes.
    uint32_t blendshape_vertex_base_ = 0;  // blendshape vertex index offset for a mesh.
  };

  static size_t GetSingleVertexSize() {
    return sizeof(Vertex);
  }

  static size_t GetSingleIndexSize() {
    return sizeof(uint32_t);
  }

  static size_t GetSingleBlendshapeVertexSize() {
    return sizeof(BlendshapeVertex);
  }

  Mesh() = default;
  virtual ~Mesh() = default;

  const std::string GetMeshName() const {
    return mesh_name_;
  }

  const size_t GetVerticesSize() const {
    return vertices_size_;
  }

  const size_t GetVerticesNumber() const {
    return vertices_number_;
  }

  const size_t GetIndicesSize() const {
    return indices_size_;
  }

  const size_t GetIndicesNumber() const {
    return indices_number_;
  }

  const size_t GetBlendshapeVerticesSize() const {
    return blendshape_vertices_size_;
  }

  const size_t GetBlendshapeVerticesNumber() const {
    return blendshape_vertices_number_;
  }

  const std::vector<Texture> GetTextures() const {
    return textures_;
  }

  const std::string GetModelName() const {
    return model_name_;
  }

  void SetMeshName(const std::string& mesh_name) {
    mesh_name_ = mesh_name;
  }

  void SetVerticesSize(size_t vertices_size) {
    vertices_size_ = vertices_size;
  }

  void SetVerticesNumber(size_t vertices_number) {
    vertices_number_ = vertices_number;
  }

  void SetIndicesSize(size_t indices_size) {
    indices_size_ = indices_size;
  }

  void SetIndicesNumber(size_t indices_number) {
    indices_number_ = indices_number;
  }

  void SetBlendshapeVerticesSize(size_t blendshape_vertices_size) {
    blendshape_vertices_size_ = blendshape_vertices_size;
  }

  void SetBlendshapeVerticesNumber(size_t blendshape_vertices_number) {
    blendshape_vertices_number_ = blendshape_vertices_number;
  }

  void InsertTexture(Texture::TextureType texture_type, const std::string& texture_file_path) {
    textures_.emplace_back(texture_type, texture_file_path);
  }

  void SetModelName(const std::string& model_name) {
    model_name_ = model_name;
  }

  virtual void GetVertexData(std::vector<Vertex>& vertices_data, size_t& vertices_size, size_t& vertices_number) = 0;
  virtual void GetIndexData(std::vector<uint32_t>& indices_data, size_t& indices_size, size_t& indices_number) = 0;
  // Get textures file paths attached to this mesh, a mesh may have multiple textures, e.g, diffuse, normal, etc.
  virtual void GetTexturesTypesAndFilePaths(std::set<Texture::TextureType>& texture_type_set, std::vector<std::string>& textures_file_paths) = 0;

  virtual void GetBlendshapeData(std::vector<BlendshapeVertex>& blendshape_vertices_data, size_t& blendshape_vertices_size, size_t& blendshape_vertices_number) = 0;

 private:
  std::string mesh_name_;

  size_t vertices_size_ = 0;
  size_t vertices_number_ = 0;

  size_t indices_size_ = 0;
  size_t indices_number_ = 0;

  size_t blendshape_vertices_size_ = 0;
  // blendshape count * vertex number per blendshape
  size_t blendshape_vertices_number_ = 0;

  std::vector<Texture> textures_;

  // The name of model which this mesh belongs to
  std::string model_name_;
};  // class Mesh

}  // namespace motion_animation