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
  void LoadVertices(const assets::MeshVertices* mesh_vertices);

  uint32_t VAO_ = 0;
  uint32_t VBO_ = 0;
  uint32_t EBO_ = 0;
  size_t total_vertices_number_ = 0;

  std::string vertex_shader_file_path = "scene_vertex.glsl";
  std::string fragment_shader_file_path = "scene_fragment.glsl";
  std::unique_ptr<Shader> gl_shader_helper_ptr_;

};  // class GLRenderer

}  // namespace motion_animation