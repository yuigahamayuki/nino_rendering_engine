#include "GL_renderer.h"

#include <algorithm>

#include "scene_manager.h"
#include "util_linear_algebra_helper.h"
#include "animation_player.h"

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
  glGenBuffers(1, &EBO_);
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
    auto scene_ptr = SceneManager::GetSharedInstance().GetCurrentScene();
    if (!scene_ptr) {
      return;
    }
    const Camera& camera = scene_ptr->GetCamera();
    Eigen::Matrix4f model(Eigen::Matrix4f::Identity());
    //Eigen::Matrix4f model;
    //model << 0.01f, 0.f, 0.f, 0.f,
    //  0.f, 0.01f, 0.f, 0.f,
    //  0.f, 0.f, 0.01f, 0.f,
    //  0.f, 0.f, 0.f, 1.f;

    Eigen::Matrix4f view;
    Eigen::Matrix4f proj;
    camera.GetViewProjMatrices(view, proj, 90.f, 1920, 1080, 1.f, 10.f);

    glm::mat4 glm_model = util::MatrixEigen2GLM(model);
    glm::mat4 glm_view = util::MatrixEigen2GLM(view);
    glm::mat4 glm_proj = util::MatrixEigen2GLM(proj);
    //gl_shader_helper_ptr_->setMat4("model", glm_model);
    //gl_shader_helper_ptr_->setMat4("view", glm_view);
    //gl_shader_helper_ptr_->setMat4("projection", glm_proj);

    gl_shader_helper_ptr_->SetEigenMat4("model", model);
    gl_shader_helper_ptr_->SetEigenMat4("view", view);
    gl_shader_helper_ptr_->SetEigenMat4("projection", proj);

    // TODO(wushiyuan): test, delete
    std::vector<Eigen::Matrix4f> bone_transforms;
    static float time_in_seconds = 0.f;
    time_in_seconds += 1.f;
    if (time_in_seconds > 171.875f) {
      time_in_seconds = 0.f;
    }
    AnimationPlayer::GetSharedInstance().ComputeBoneTransforms(time_in_seconds,
      bone_transforms,
      AnimationManager::GetSharedInstance().GetBonesMapForModel("miruku"),
      AnimationManager::GetSharedInstance().GetBonesInfoForModel("miruku"),
      AnimationManager::GetSharedInstance().GetAnimationForModel("miruku", "Armature|ArmatureAction.001"),
      AnimationManager::GetSharedInstance().GetAnimationRootNodeForModel("miruku"));

    for (size_t i = 0; i < bone_transforms.size(); ++i) {
      const std::string uniform_name = std::string("bone_transforms") + "[" + std::to_string(i) + "]";
      gl_shader_helper_ptr_->SetEigenMat4(uniform_name, bone_transforms[i]);
    }

    glBindVertexArray(VAO_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
    std::vector<Mesh::DrawArugument> all_models_draw_arguments;
    scene_ptr->GetAllModelsDrawArguments(all_models_draw_arguments);
    std::for_each(all_models_draw_arguments.cbegin(), all_models_draw_arguments.cend(), [this](const Mesh::DrawArugument& draw_argument) {
      glDrawElementsInstancedBaseVertex(GL_TRIANGLES, draw_argument.index_count_, GL_UNSIGNED_INT, reinterpret_cast<void*>(draw_argument.index_start_ * sizeof(uint32_t)), 1, draw_argument.vertex_base_);
    });
  }
}

void GLRenderer::LoadAssets(const std::vector<std::unique_ptr<assets::Asset>>& assets) {
  std::for_each(assets.cbegin(), assets.cend(), [this](const std::unique_ptr<assets::Asset>& asset) {
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

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh_vertices->model_all_meshes_indices_size_, mesh_vertices->model_all_meshes_indices_data_.data(), GL_STATIC_DRAW);
}


}  // namespace motion_animation