#pragma once

#include "renderer.h"

#include <cstdint>

#include <glad/glad.h>

namespace motion_animation {

class GLRenderer : public Renderer {
public:
  GLRenderer();
  ~GLRenderer();

  void Initialize() override;
  void Destroy() override;
  void Render() override;
  void LoadAssets(const std::vector<std::unique_ptr<assets::Assets>>& assets) override;

private:
  void LoadVertices(const assets::MeshVertices* mesh_vertices);

  uint32_t VAO_ = 0;
  uint32_t VBO_ = 0;

};  // class GLRenderer

}  // namespace motion_animation