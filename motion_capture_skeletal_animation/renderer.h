#pragma once

namespace motion_animation {

class Renderer {
 public:
  Renderer() = default;
  virtual ~Renderer() = default;

  Renderer(const Renderer&) = delete;
  Renderer& operator=(const Renderer&) = delete;

  virtual void Initialize() = 0;
  virtual void Update() = 0;
  virtual void Render() = 0;
  virtual void LoadAssets() = 0;

 private:

};  // class Renderer

}  // namespace motion_animation