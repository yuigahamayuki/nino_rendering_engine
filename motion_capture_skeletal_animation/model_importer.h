#pragma once

#include <string>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>

namespace motion_animation {

class ModelImporter {
 public:
   static ModelImporter& GetSharedInstance();

   ~ModelImporter() = default;

   ModelImporter(const ModelImporter&) = delete;
   ModelImporter& operator=(const ModelImporter&) = delete;

   bool LoadModelFile(const std::string& model_file_path);
 private:
   ModelImporter();

   Assimp::Importer assimp_importer_;
   // Assimp::Importer takes ownership of aiScene, so unique_ptr is not necessary
   const aiScene* assimp_scene_ptr_ = nullptr;
};  // class ModelImporter

}  // namespace