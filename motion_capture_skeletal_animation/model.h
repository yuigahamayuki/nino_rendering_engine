#pragma once

#include <string>
#include <vector>
#include <memory>

#include "mesh.h"

namespace motion_animation {

class Model {
 public:
  Model() = default;
  Model(const std::string& model_name) : model_name_(model_name) {

  }
  ~Model() = default;

  Model(const Model&) = delete;
  Model& operator=(const Model&) = delete;

  Model(Model&&) = default;
  Model& operator=(Model&&) = default;

 private:
  std::string model_name_;

  std::vector<std::unique_ptr<Mesh>> meshes_;
};  // class Model

}  // namespace motion_animation