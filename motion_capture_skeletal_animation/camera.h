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

  void Set(const Eigen::Vector3f& camera_pos, const Eigen::Vector3f& look_direction, const Eigen::Vector3f& up) {
    camera_pos_ = camera_pos;
    look_direction_ = look_direction;
    up_ = up;
    UpdateCameraVectors();
  }

  // Note(wushiyuan): view space: right hand; near_plane projects to -1, far_plane projects to 1.
  void GetViewProjMatrices(Eigen::Matrix4f& view, Eigen::Matrix4f& proj, float fov_in_degrees, float screen_width, float screen_height, float near_z, float far_z) const;

  // Camera operation responding to user input.
  void Update();

 private:
   void RotateLeftRight(float angle_degree);
   void RotateUpDown(float angle_degree);
   void MoveForwardBackwoard(float move_units);
   void MoveLeftRight(float move_units);

   void UpdateCameraVectors();

  Eigen::Vector3f camera_pos_;
  Eigen::Vector3f look_direction_;  // look_target - camera_pos
  Eigen::Vector3f right_;
  Eigen::Vector3f up_;
  float yaw_ = -90.f;
  float pitch_ = 0.f;
};  // class Camera

}  // namespace motion_animation