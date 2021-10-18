#include "GL_renderer.h"

#include <algorithm>
#include <set>

#include "scene_manager.h"
#include "util_linear_algebra_helper.h"
#include "util_image_loader.h"
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
    auto scene_ptr = SceneManager::GetSharedInstance().GetCurrentScene();
    if (!scene_ptr) {
      return;
    }
    const Camera& camera = scene_ptr->GetCamera();
    Eigen::Matrix4f model(Eigen::Matrix4f::Identity());

    Eigen::Matrix4f view;
    Eigen::Matrix4f proj;
    camera.GetViewProjMatrices(view, proj, 60.f, 1920, 1080, 0.01f, 10.f);

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
    /*std::vector<Eigen::Matrix4f> bone_transforms;
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
    }*/

    glBindVertexArray(VAO_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
    std::vector<Mesh::DrawArugument> all_models_draw_arguments;
    scene_ptr->GetAllModelsDrawArguments(all_models_draw_arguments);
    std::for_each(all_models_draw_arguments.cbegin(), all_models_draw_arguments.cend(), [this](const Mesh::DrawArugument& draw_argument) {
      for (size_t i = 0; i < draw_argument.textures_.size(); ++i) {
        BindTexture(draw_argument.textures_[i]);
      }
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
      case assets::AssetType::texture: {
        assets::Textures* textures_asset = reinterpret_cast<assets::Textures*>(asset.get());
        LoadTextures(textures_asset);
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
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(attribute_stride), (void*)attribute_offset);
  glEnableVertexAttribArray(0);
  attribute_offset += 3 * sizeof(float);
  // normal attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(attribute_stride), (void*)attribute_offset);
  glEnableVertexAttribArray(1);
  attribute_offset += 3 * sizeof(float);
  // texture coordinate attribtue
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(attribute_stride), (void*)attribute_offset);
  glEnableVertexAttribArray(2);
  attribute_offset += 2 * sizeof(float);
  // bone id attribute
  glVertexAttribIPointer(3, 4, GL_INT, static_cast<GLsizei>(attribute_stride), (void*)attribute_offset);
  glEnableVertexAttribArray(3);
  attribute_offset += 4 * sizeof(int);
  // bone weight attribute
  glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(attribute_stride), (void*)attribute_offset);
  glEnableVertexAttribArray(4);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh_vertices->model_all_meshes_indices_size_, mesh_vertices->model_all_meshes_indices_data_.data(), GL_STATIC_DRAW);

  // blendshape related buffer objects
  if (mesh_vertices->model_all_blendshape_vertices_number_ > 0) {
    glGenBuffers(1, &SSBO_blendshape_vertices_);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO_blendshape_vertices_);
    glBufferData(GL_SHADER_STORAGE_BUFFER, mesh_vertices->model_all_blendshape_vertices_size_, mesh_vertices->model_all_meshes_blendshape_vertices_data_.data(), GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, SSBO_blendshape_vertices_);

    glGenBuffers(1, &UBO_blendshape_weights_);
    glBindBuffer(GL_UNIFORM_BUFFER, UBO_blendshape_weights_);
    glBufferData(GL_UNIFORM_BUFFER, mesh_vertices->blendshape_number_ * sizeof(float), nullptr, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, UBO_blendshape_weights_);
  }
}

void GLRenderer::LoadTextures(const assets::Textures* textures_asset) {
  std::set<std::string> textures_file_paths_set;

  for (size_t i = 0; i < textures_asset->textures_file_paths_.size(); ++i) {
    const std::string& texture_file_path = textures_asset->textures_file_paths_[i];
    if (textures_file_paths_set.find(texture_file_path) == textures_file_paths_set.end()) {
      // *** Read texture file from disk. ***
      util::ImageLoader::GLTextureDesc gl_texture_desc;
      int bytes_per_row = 0;
      std::vector<uint8_t> image_data;
      auto image_size = util::ImageLoader::LoadImageDataFromFile(image_data, gl_texture_desc, texture_file_path, bytes_per_row);

      // *** Upload texture to GPU. ***
      if (image_size > 0) {
        GLuint texture_id = 0;
        glGenTextures(1, &texture_id);
        glBindTexture(GL_TEXTURE_2D, texture_id);
        texture_ids_.emplace_back(texture_id);
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, gl_texture_desc.width, gl_texture_desc.height, 0, gl_texture_desc.format, GL_UNSIGNED_BYTE, image_data.data());
        glGenerateMipmap(GL_TEXTURE_2D);

        texture_file_path_id_map_.emplace(texture_file_path, texture_id);
      } else {
        std::cout << "Failed to load texture" << std::endl;
      }

      // Do not load the same texture twice.
      textures_file_paths_set.emplace(texture_file_path);
    } else {

    }
  }

  std::string fragment_macro;
  bool has_texture_types_other_than_diffuse = false;
  if (textures_asset->texture_type_set_.find(Mesh::Texture::TextureType::alpha) != textures_asset->texture_type_set_.end()) {
    fragment_macro += std::string("#define has_alpha_texture\n");
    has_texture_types_other_than_diffuse = true;
    std::cout << "model has alpha texture(s)" << std::endl;
  }

  if (has_texture_types_other_than_diffuse) {
    gl_shader_helper_ptr_ = std::make_unique<Shader>(vertex_shader_file_path.c_str(), fragment_shader_file_path.c_str(), nullptr, nullptr, fragment_macro.c_str());
  } else {
    gl_shader_helper_ptr_ = std::make_unique<Shader>(vertex_shader_file_path.c_str(), fragment_shader_file_path.c_str());
  }

  gl_shader_helper_ptr_->use();
  // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
  gl_shader_helper_ptr_->setInt("diffuse_sampler", static_cast<int>(TextureUnit::diffuse_texture_unit));
  gl_shader_helper_ptr_->setInt("specular_sampler", static_cast<int>(TextureUnit::specular_texture_unit));
  gl_shader_helper_ptr_->setInt("normal_sampler", static_cast<int>(TextureUnit::normal_texture_unit));
  gl_shader_helper_ptr_->setInt("alpha_sampler", static_cast<int>(TextureUnit::alpha_texture_unit));
}

void GLRenderer::BindTexture(const Mesh::Texture& texture) {
  GLenum texture_unit = GL_TEXTURE0;
  
  switch (texture.texture_type_) {
    case Mesh::Texture::TextureType::diffuse: {
      texture_unit = GL_TEXTURE0;
      break;
    }
    case Mesh::Texture::TextureType::specular: {
      texture_unit = GL_TEXTURE1;
      break;
    }
    case Mesh::Texture::TextureType::normal: {
      texture_unit = GL_TEXTURE2;
      break;
    }
    case Mesh::Texture::TextureType::alpha: {
      texture_unit = GL_TEXTURE3;
      break;
    }
    default: {
      break;
    }
  }

  glActiveTexture(texture_unit);
  auto texture_id = texture_file_path_id_map_[texture.texture_file_path_];
  glBindTexture(GL_TEXTURE_2D, texture_id);
}


}  // namespace motion_animation