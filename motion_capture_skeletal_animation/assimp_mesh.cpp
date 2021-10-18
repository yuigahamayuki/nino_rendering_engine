#include "assimp_mesh.h"

#include "animation_manager.h"

#include "util_linear_algebra_helper.h"

namespace {

motion_animation::Mesh::Texture::TextureType ConvertAssimpTextureType(const aiTextureType& assimp_texture_type) {
  motion_animation::Mesh::Texture::TextureType texture_type = motion_animation::Mesh::Texture::TextureType::diffuse;

  switch (assimp_texture_type) {
    case aiTextureType_DIFFUSE: {
      texture_type = motion_animation::Mesh::Texture::TextureType::diffuse;
      break;
    }
    case aiTextureType_SPECULAR: {
      texture_type = motion_animation::Mesh::Texture::TextureType::specular;
      break;
    }
    case aiTextureType_NORMALS: {
      texture_type = motion_animation::Mesh::Texture::TextureType::normal;
      break;
    }
    case aiTextureType_OPACITY: {
      texture_type = motion_animation::Mesh::Texture::TextureType::alpha;
      break;
    }
    default: {
      break;
    }
  }

  return texture_type;
}

}  // namespace

namespace {

void InsertBoneIdAndBoneWeightToVertexData(std::vector<motion_animation::Mesh::Vertex>& vertices_data, uint32_t vertex_index, int bone_index, float bone_weight) {
  if (vertex_index < vertices_data.size()) {
    for (auto i = 0; i < _countof(vertices_data[vertex_index].bone_ids_); ++i) {
      if (vertices_data[vertex_index].bone_ids_[i] < 0) {
        vertices_data[vertex_index].bone_ids_[i] = bone_index;
        vertices_data[vertex_index].bone_weights_[i] = bone_weight;
        break;
      }

    }

  }

}

}  // namespace

namespace motion_animation {

AssimpMesh::AssimpMesh(const aiScene* assimp_scene_ptr, uint32_t mesh_index_of_assimp, const std::string& model_name)
  : assimp_scene_ptr_(assimp_scene_ptr), mesh_index_of_assimp_(mesh_index_of_assimp) {

  auto total_mesh_number = assimp_scene_ptr_->mNumMeshes;
  if (mesh_index_of_assimp_ < total_mesh_number) {
    SetMeshName(assimp_scene_ptr_->mMeshes[mesh_index_of_assimp_]->mName.C_Str());
    SetModelName(model_name);
  }
}

void AssimpMesh::GetVertexData(std::vector<Vertex>& vertices_data, size_t& vertices_size, size_t& vertices_number) {
  vertices_data.clear();
  vertices_size = 0;
  vertices_number = 0;
  if (!assimp_scene_ptr_) {
    return;
  }

  auto total_mesh_number = assimp_scene_ptr_->mNumMeshes;
  if (mesh_index_of_assimp_ < total_mesh_number) {
    auto mesh = assimp_scene_ptr_->mMeshes[mesh_index_of_assimp_];

    vertices_number = mesh->mNumVertices;
    vertices_size = Mesh::GetSingleVertexSize() * vertices_number;

    SetVerticesNumber(vertices_number);
    SetVerticesSize(vertices_size);

    for (uint32_t i = 0; i < mesh->mNumVertices; ++i) {
      Vertex vertex;
      vertex.position_[0] = mesh->mVertices[i].x;
      vertex.position_[1] = mesh->mVertices[i].y;
      vertex.position_[2] = mesh->mVertices[i].z;
      if (mesh->mNormals) {
        vertex.normal_[0] = mesh->mNormals[i].x;
        vertex.normal_[1] = mesh->mNormals[i].y;
        vertex.normal_[2] = mesh->mNormals[i].z;
      }
      if (mesh->mTextureCoords[0]) {
        vertex.texture_coordinates_[0] = mesh->mTextureCoords[0][i].x;
        vertex.texture_coordinates_[1] = mesh->mTextureCoords[0][i].y;
      }
      
      vertices_data.emplace_back(vertex);
    }

    for (uint32_t i = 0; i < mesh->mNumBones; ++i) {
      auto bone = mesh->mBones[i];
      const std::string bone_name = bone->mName.C_Str();
      bool bone_is_exist = false;
      uint32_t bone_index = AnimationManager::GetSharedInstance().FindBoneIndexForModel(GetModelName(), bone_name, bone_is_exist);
      if (!bone_is_exist) {
        Eigen::Matrix4f offset_matrix = util::MatrixAssimp2Eigen(bone->mOffsetMatrix);
        AnimationManager::GetSharedInstance().InsertBoneInfoForModel(GetModelName(), AnimationBone(bone_name, offset_matrix));
      }
      for (uint32_t j = 0; j < bone->mNumWeights; ++j) {
        auto vertex_id = bone->mWeights[j].mVertexId;
        auto bone_weight = bone->mWeights[j].mWeight;

        InsertBoneIdAndBoneWeightToVertexData(vertices_data, vertex_id, bone_index, bone_weight);
      }
    }

  }

}

void AssimpMesh::GetIndexData(std::vector<uint32_t>& indices_data, size_t& indices_size, size_t& indices_number) {
  indices_data.clear();
  indices_size = 0;
  indices_number = 0;
  if (!assimp_scene_ptr_) {
    return;
  }

  auto total_mesh_number = assimp_scene_ptr_->mNumMeshes;
  if (mesh_index_of_assimp_ < total_mesh_number) {
    auto mesh = assimp_scene_ptr_->mMeshes[mesh_index_of_assimp_];
    for (uint32_t i = 0; i < mesh->mNumFaces; ++i) {
      auto face = mesh->mFaces[i];
      indices_size += Mesh::GetSingleIndexSize() * face.mNumIndices;
      indices_number += face.mNumIndices;
      for (uint32_t j = 0; j < face.mNumIndices; ++j) {
        indices_data.emplace_back(face.mIndices[j]);
      }
    }

    SetIndicesSize(indices_size);
    SetIndicesNumber(indices_number);
  }
}

void AssimpMesh::GetTexturesTypesAndFilePaths(std::set<Texture::TextureType>& texture_type_set, std::vector<std::string>& textures_file_paths) {
  textures_file_paths.clear();
  if (!assimp_scene_ptr_) {
    return;
  }

  auto total_mesh_number = assimp_scene_ptr_->mNumMeshes;
  if (mesh_index_of_assimp_ < total_mesh_number) {
    auto material_index = assimp_scene_ptr_->mMeshes[mesh_index_of_assimp_]->mMaterialIndex;
    auto total_material_number = assimp_scene_ptr_->mNumMaterials;
    if (material_index < total_material_number) {
      const aiMaterial* material = assimp_scene_ptr_->mMaterials[material_index];
      // TODO(wushiyuan): may add more texture types
      GetTexturesFilePathsForTextureType(material, aiTextureType_DIFFUSE, texture_type_set, textures_file_paths);
      GetTexturesFilePathsForTextureType(material, aiTextureType_SPECULAR, texture_type_set, textures_file_paths);
      GetTexturesFilePathsForTextureType(material, aiTextureType_NORMALS, texture_type_set, textures_file_paths);
      GetTexturesFilePathsForTextureType(material, aiTextureType_OPACITY, texture_type_set, textures_file_paths);
    }
  }
}

void AssimpMesh::GetBlendshapeData(std::vector<BlendshapeVertex>& blendshape_vertices_data, size_t& blendshape_vertices_size, size_t& blendshape_vertices_number) {
  blendshape_vertices_data.clear();
  blendshape_vertices_size = 0;
  blendshape_vertices_number = 0;
  if (!assimp_scene_ptr_) {
    return;
  }

  auto total_mesh_number = assimp_scene_ptr_->mNumMeshes;
  if (mesh_index_of_assimp_ < total_mesh_number) {
    auto mesh = assimp_scene_ptr_->mMeshes[mesh_index_of_assimp_];
    uint32_t blendshape_count = mesh->mNumAnimMeshes;
    if (blendshape_count > 0) {
      for (uint32_t i = 0; i < blendshape_count; ++i) {
        auto blendshape_mesh = mesh->mAnimMeshes[i];
        std::string blendshape_name = blendshape_mesh->mName.C_Str();
        // TODO(wushiyuan): I only need IOS ARKit blendshapes,
        // the model I downloaded contains other blendshapes whose names
        // start with uppercase letter, while ARKit blendshapes' names
        // start with lowercase letter.
        if (blendshape_name[0] >= 'a' && blendshape_name[0] <= 'z') {
          // The vertex count of blendshape mesh and that of original mesh are supposed to be the same.
          for (uint32_t j = 0; j < blendshape_mesh->mNumVertices; ++j) {
            BlendshapeVertex blendshape_vertex;
            // Note(wushiyuan): upload the difference of vector.
            blendshape_vertex.position_[0] = blendshape_mesh->mVertices[j].x - mesh->mVertices[j].x;
            blendshape_vertex.position_[1] = blendshape_mesh->mVertices[j].y - mesh->mVertices[j].y;
            blendshape_vertex.position_[2] = blendshape_mesh->mVertices[j].z - mesh->mVertices[j].z;
            if (blendshape_mesh->mNormals && mesh->mNormals) {
              blendshape_vertex.normal_[0] = blendshape_mesh->mNormals[j].x - mesh->mNormals[j].x;
              blendshape_vertex.normal_[1] = blendshape_mesh->mNormals[j].y - mesh->mNormals[j].y;
              blendshape_vertex.normal_[2] = blendshape_mesh->mNormals[j].z - mesh->mNormals[j].z;
            }

            blendshape_vertices_data.emplace_back(blendshape_vertex);
            blendshape_vertices_number++;
            blendshape_vertices_size += GetSingleBlendshapeVertexSize();
          }
        }
      }

      // blendshape vertices number = blendshape count * vertex number per blendshape
      SetBlendshapeVerticesNumber(blendshape_vertices_number);
      SetBlendshapeVerticesSize(blendshape_vertices_size);
    }
  }
}

void AssimpMesh::GetTexturesFilePathsForTextureType(const aiMaterial* material, aiTextureType assimp_texture_type, std::set<Texture::TextureType>& texture_type_set, std::vector<std::string>& textures_file_paths) {
  for (uint32_t i = 0; i < material->GetTextureCount(assimp_texture_type); ++i) {
    aiString texture_file_path_ai_string;
    material->GetTexture(assimp_texture_type, i, &texture_file_path_ai_string);
    std::string texture_file_path(texture_file_path_ai_string.C_Str());
    if (!texture_file_path.empty()) {
      Texture::TextureType texture_type = ConvertAssimpTextureType(assimp_texture_type);
      texture_type_set.insert(texture_type);
      textures_file_paths.emplace_back(texture_file_path);
      InsertTexture(texture_type, texture_file_path);
    }
  }
}

}  // namespace motion_animation