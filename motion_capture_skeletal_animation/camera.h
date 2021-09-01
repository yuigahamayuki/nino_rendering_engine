#pragma once

#include <Eigen/Eigen>

namespace motion_animation {

class Camera {
 public:
  Camera() = default;
  ~Camera() = default;

  static Eigen::Matrix4f ComputeViewMatrixRH(const Eigen::Vector3f& camera_pos, const Eigen::Vector3f& look_target, const Eigen::Vector3f& up);
  // Note(wushiyuan):
  // (1) near_z and far_z all > 0, for right hand, the actural z coordinates are -near_z and -far_z, respectively.
  // (2) NDC is the same as OpenGL's, -near_z -> -1, -far_z -> 1
  static Eigen::Matrix4f ComputePerspectiveMatrixFovRH_OpenGLNDC(float fov_in_radius, float aspect_ratio, float near_z, float far_z);

  void Set(const Eigen::Vector3f& camera_pos, const Eigen::Vector3f& look_target, const Eigen::Vector3f& up) {
    camera_pos_ = camera_pos;
    look_target_ = look_target;
    up_ = up;
  }

  // Note(wushiyuan): view space: right hand; near_plane projects to -1, far_plane projects to 1.
  void GetViewProjMatrices(Eigen::Matrix4f& view, Eigen::Matrix4f& proj, float fov_in_degrees, float screen_width, float screen_height, float near_z, float far_z) const;

 private:
  Eigen::Vector3f camera_pos_;
  Eigen::Vector3f look_target_;
  Eigen::Vector3f up_;
};  // class Camera

}  // namespace motion_animation