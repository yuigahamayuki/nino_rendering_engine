#pragma once

#include "renderer.h"

#include <cstdint>

#include <glad/glad.h>

#include "GL_shader_helper.h"

#include <memory>

namespace motion_animation {

class GLRenderer : public Renderer {
public:
  GLRenderer();
  ~GLRenderer();

  void Initialize() override;
  void Destroy() override;
  void Render() override;
  void LoadAssets(const std::vector<std::unique_ptr<assets::Asset>>& assets) override;

private:
  // Sampler slot
  enum class TextureUnit {
    diffuse_texture_unit = 0,
    specular_texture_unit = 1,
    normal_texture_unit = 2,
    alpha_texture_unit = 3,
  };
  void LoadVertices(const assets::MeshVertices* mesh_vertices);
  void LoadTextures(const assets::Textures* textures_asset);

  void BindTexture(const Mesh::Texture& texture);

  uint32_t VAO_ = 0;
  uint32_t VBO_ = 0;
  uint32_t EBO_ = 0;
  uint32_t SSBO_blendshape_vertices_ = 0;

  std::vector<GLuint> texture_ids_;
  std::map<std::string, GLuint> texture_file_path_id_map_;

  std::string vertex_shader_file_path = "scene_vertex.glsl";
  std::string fragment_shader_file_path = "scene_fragment.glsl";
  std::unique_ptr<Shader> gl_shader_helper_ptr_;

};  // class GLRenderer

}  // namespace motion_animation