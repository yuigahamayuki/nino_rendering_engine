#include "GL_renderer.h"

#include <algorithm>

namespace motion_animation {

GLRenderer::GLRenderer() {
  Initialize();
}

GLRenderer::~GLRenderer() {
  Destroy();
}

void GLRenderer::Initialize() {
  // configure global opengl state
  // -----------------------------
  glEnable(GL_DEPTH_TEST);

  gl_shader_helper_ptr_ = std::make_unique<Shader>(vertex_shader_file_path.c_str(), fragment_shader_file_path.c_str());

  glGenVertexArrays(1, &VAO_);
  glGenBuffers(1, &VBO_);
}

void GLRenderer::Destroy() {
  glDeleteVertexArrays(1, &VAO_);
  glDeleteBuffers(1, &VBO_);
}

void GLRenderer::Render() {
  if (gl_shader_helper_ptr_) {
    gl_shader_helper_ptr_->use();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // TODO(wushiyuan): implement more
    glDrawArrays(GL_TRIANGLES, 0, total_vertices_number_);
  }
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
  glBindVertexArray(VAO_);

  glBindBuffer(GL_ARRAY_BUFFER, VBO_);
  glBufferData(GL_ARRAY_BUFFER, mesh_vertices->model_all_meshes_vertices_size_, mesh_vertices->model_all_meshes_vertices_data_.data(), GL_STATIC_DRAW);

  size_t attribute_stride = Mesh::GetSingleVertexSize();
  size_t attribute_offset = 0;
  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, attribute_stride, (void*)attribute_offset);
  glEnableVertexAttribArray(0);
  attribute_offset += 3 * sizeof(float);
  // normal attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, attribute_stride, (void*)attribute_offset);
  glEnableVertexAttribArray(1);
  attribute_offset += 3 * sizeof(float);
  // texture coordinate attribtue
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, attribute_stride, (void*)attribute_offset);
  glEnableVertexAttribArray(2);
  attribute_offset += 2 * sizeof(float);
  // bone id attribute
  glVertexAttribIPointer(3, 4, GL_INT, attribute_stride, (void*)attribute_offset);
  glEnableVertexAttribArray(3);
  attribute_offset += 4 * sizeof(int);
  // bone weight attribute
  glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, attribute_stride, (void*)attribute_offset);
  glEnableVertexAttribArray(4);

  total_vertices_number_ = mesh_vertices->model_all_meshes_vertices_number_;
}


}  // namespace motion_animation