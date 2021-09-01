#pragma once

#include <vector>
#include <memory>

#include "assets.h"

namespace motion_animation {

class Renderer {
 public:
  Renderer() = default;
  virtual ~Renderer() = default;

  Renderer(const Renderer&) = delete;
  Renderer& operator=(const Renderer&) = delete;

  virtual void Initialize() = 0;
  virtual void Destroy() = 0;
  virtual void Render() = 0;
  virtual void LoadAssets(const std::vector<std::unique_ptr<assets::Assets>>& assets) = 0;

 private:

};  // class Renderer

}  // namespace motion_animation