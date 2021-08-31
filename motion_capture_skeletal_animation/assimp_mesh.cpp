#include "assimp_mesh.h"

#include "animation_manager.h"

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

AssimpMesh::AssimpMesh(const aiScene* assimp_scene_ptr, uint32_t mesh_index_of_assimp)
  : assimp_scene_ptr_(assimp_scene_ptr), mesh_index_of_assimp_(mesh_index_of_assimp) {

  auto total_mesh_number = assimp_scene_ptr_->mNumMeshes;
  if (mesh_index_of_assimp_ < total_mesh_number) {
    SetMeshName(assimp_scene_ptr_->mMeshes[mesh_index_of_assimp_]->mName.C_Str());
  }
}

void AssimpMesh::GetVertexData(std::vector<Vertex>& vertices_data, size_t& vertices_size, size_t& vertices_number) {
  vertices_data.clear();
  vertices_size = 0;
  vertices_number = 0;
  auto total_mesh_number = assimp_scene_ptr_->mNumMeshes;
  if (mesh_index_of_assimp_ < total_mesh_number) {
    auto mesh = assimp_scene_ptr_->mMeshes[mesh_index_of_assimp_];

    vertices_number = mesh->mNumVertices;
    vertices_size = sizeof(Vertex) * vertices_number;

    SetDiffuseTextureName(mesh->mTextureCoordsNames[0].C_Str());

    for (uint32_t i = 0; i < mesh->mNumVertices; ++i) {
      Vertex vertex;
      vertex.positions_[0] = mesh->mVertices[i].x;
      vertex.positions_[1] = mesh->mVertices[i].y;
      vertex.positions_[2] = mesh->mVertices[i].z;
      vertex.normals_[0] = mesh->mNormals[i].x;
      vertex.normals_[1] = mesh->mNormals[i].y;
      vertex.normals_[2] = mesh->mNormals[i].z;
      vertex.texture_coordinates_[0] = mesh->mTextureCoords[0][i].x;
      vertex.texture_coordinates_[1] = mesh->mTextureCoords[0][i].y;
      
      vertices_data.emplace_back(vertex);
    }

    for (uint32_t i = 0; i < mesh->mNumBones; ++i) {
      auto bone = mesh->mBones[i];
      const std::string bone_name = bone->mName.C_Str();
      uint32_t bone_index = AnimationManager::GetSharedInstance().FindBoneIndexForNameOfModel("ruby", bone_name); 
      // TODO: use map to determine bone_index
      for (uint32_t j = 0; j < bone->mNumWeights; ++j) {
        auto vertex_id = bone->mWeights[j].mVertexId;
        auto bone_weight = bone->mWeights[j].mWeight;

        InsertBoneIdAndBoneWeightToVertexData(vertices_data, vertex_id, bone_index, bone_weight);
      }
    }

  }

}

}  // namespace motion_animation