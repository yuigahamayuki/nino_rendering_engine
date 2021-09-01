#include "GL_renderer.h"

#include <algorithm>

namespace motion_animation {

GLRenderer::GLRenderer(){
  Initialize();
}

GLRenderer::~GLRenderer() {
  Destroy();
}

void GLRenderer::Initialize() {
  // configure global opengl state
  // -----------------------------
  glEnable(GL_DEPTH_TEST);

  // TODO(wushiyuan): compile shader program

  glGenVertexArrays(1, &VAO_);
  glGenBuffers(1, &VBO_);
}

void GLRenderer::Destroy() {
  glDeleteVertexArrays(1, &VAO_);
  glDeleteBuffers(1, &VBO_);
}

void GLRenderer::Render() {
}

void GLRenderer::LoadAssets(const std::vector<std::unique_ptr<assets::Assets>>& assets) {
  std::for_each(assets.cbegin(), assets.cend(), [this](const std::unique_ptr<assets::Assets>& asset) {
    switch (asset->asset_type()) {
      case assets::AssetType::vertex: {
        assets::MeshVertices* mesh_vertices = reinterpret_cast<assets::MeshVertices*>(asset.get());
        LoadVertices(mesh_vertices);
        break;
      }
      default:
        break;
    }
  });
}

void GLRenderer::LoadVertices(const assets::MeshVertices* mesh_vertices) {
  int a = 1;
}


}  // namespace motion_animation