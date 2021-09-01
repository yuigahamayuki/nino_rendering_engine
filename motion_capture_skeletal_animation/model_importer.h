#pragma once

#include <string>
#include <map>
#include <memory>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>

#include "mesh.h"

namespace motion_animation {

class ModelImporter {
 public:
   static ModelImporter& GetSharedInstance();

   ~ModelImporter() = default;

   ModelImporter(const ModelImporter&) = delete;
   ModelImporter& operator=(const ModelImporter&) = delete;

   bool LoadModelFile(const std::string& model_file_path);
   void LoadAllMeshesForModel(const std::string& model_name, std::vector<std::unique_ptr<Mesh>>& meshes);
 private:
   ModelImporter();

   Assimp::Importer assimp_importer_;
   // Assimp::Importer takes ownership of aiScene, so unique_ptr is not necessary
   const aiScene* assimp_scene_ptr_ = nullptr;
};  // class ModelImporter

}  // namespace