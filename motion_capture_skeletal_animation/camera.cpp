#include "camera.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include "input_manager.h"

namespace {

float Degree2Radian(float degree) {
  return degree * M_PI / 180.f;
}

}  // namespace

namespace motion_animation {

Eigen::Matrix4f Camera::ComputeViewMatrixRH(const Eigen::Vector3f& camera_pos, const Eigen::Vector3f& look_target, const Eigen::Vector3f& up) {
  // Note(wushiyuan): For right hand coordinate system of view space, z-axis of view space aligns with vector look_target towards camera_pos
  Eigen::Vector3f forward = (camera_pos - look_target).normalized();  // aligns with z-axis
  Eigen::Vector3f right = (up.cross(forward)).normalized();   // aligns with x-axis
  Eigen::Vector3f refined_up = forward.cross(right);    // aligns with y-axis

  Eigen::Vector3f negative_camera_pos = -1.f * camera_pos;

  Eigen::Matrix4f view_matrix;
  view_matrix << right.x(), right.y(), right.z(), right.dot(negative_camera_pos),
                  refined_up.x(), refined_up.y(), refined_up.z(), refined_up.dot(negative_camera_pos),
                  forward.x(), forward.y(), forward.z(), forward.dot(negative_camera_pos),
                  0.f, 0.f, 0.f, 1.f;

  return view_matrix;
}

Eigen::Matrix4f Camera::ComputePerspectiveMatrixFovRH_OpenGLNDC(float fov_in_radius, float aspect_ratio, float near_z, float far_z) {
  float c = 1.f / tan(fov_in_radius / 2.f);
  float A = (near_z + far_z) / (near_z - far_z);
  float B = 2.f * near_z * far_z / (near_z - far_z);

  Eigen::Matrix4f perspective_projection_matrix;
  perspective_projection_matrix << c / aspect_ratio, 0.f, 0.f, 0.f,
    0.f, c, 0.f, 0.f,
    0.f, 0.f, A, B,
    0.f, 0.f, -1.f, 0.f;

  return perspective_projection_matrix;
}

void Camera::GetViewProjMatrices(Eigen::Matrix4f& view, Eigen::Matrix4f& proj, float fov_in_degrees, float screen_width, float screen_height, float near_z, float far_z) const {
  Eigen::Vector3f look_target = camera_pos_ + look_direction_;
  view = ComputeViewMatrixRH(camera_pos_, look_target, up_);

  float fov_in_radius = Degree2Radian(fov_in_degrees);
  float aspect_ratio = screen_width / screen_height;

  proj = ComputePerspectiveMatrixFovRH_OpenGLNDC(fov_in_radius, aspect_ratio, near_z, far_z);
}

void Camera::Update() {
  // TODO(wushiyuan): rotation angle changes with elapsed time.
  static const float kRotationChange = 1.f;
  static const float kMoveSpeed = 0.05f;
  // Rotate camera.
  bool should_update_camera = false;
  if (InputManager::GetSharedInstance().GetKeyPressStateOfKey(InputManager::KeyboardKey::left)) {
    RotateLeftRight(kRotationChange);
    should_update_camera = true;
  }
  if (InputManager::GetSharedInstance().GetKeyPressStateOfKey(InputManager::KeyboardKey::right)) {
    RotateLeftRight(-kRotationChange);
    should_update_camera = true;
  }
  if (InputManager::GetSharedInstance().GetKeyPressStateOfKey(InputManager::KeyboardKey::up)) {
    RotateUpDown(kRotationChange);
    should_update_camera = true;
  }
  if (InputManager::GetSharedInstance().GetKeyPressStateOfKey(InputManager::KeyboardKey::down)) {
    RotateUpDown(-kRotationChange);
    should_update_camera = true;
  }

  if (should_update_camera) {
    UpdateCameraVectors();
  }

  // Move camera.
  if (InputManager::GetSharedInstance().GetKeyPressStateOfKey(InputManager::KeyboardKey::w)) {
    MoveForwardBackwoard(kMoveSpeed);
  }
  if (InputManager::GetSharedInstance().GetKeyPressStateOfKey(InputManager::KeyboardKey::s)) {
    MoveForwardBackwoard(-kMoveSpeed);
  }
  if (InputManager::GetSharedInstance().GetKeyPressStateOfKey(InputManager::KeyboardKey::a)) {
    MoveLeftRight(-kMoveSpeed);
  }
  if (InputManager::GetSharedInstance().GetKeyPressStateOfKey(InputManager::KeyboardKey::d)) {
    MoveLeftRight(kMoveSpeed);
  }
}

void Camera::RotateLeftRight(float angle_degree) {
  yaw_ += angle_degree;
}

void Camera::RotateUpDown(float angle_degree) {
  pitch_ += angle_degree;

  // Constrain pitch.
  if (pitch_ > 89.f) {
    pitch_ = 89.f;
  }
  if (pitch_ < -89.f) {
    pitch_ = -89.f;
  }
}

void Camera::MoveForwardBackwoard(float move_units) {
  camera_pos_ += look_direction_ * move_units;
}

void Camera::MoveLeftRight(float move_units) {
  camera_pos_ += right_ * move_units;
}

void Camera::UpdateCameraVectors() {
  look_direction_ = Eigen::Vector3f(
    cos(Degree2Radian(yaw_)) * cos(Degree2Radian(pitch_)),
    sin(Degree2Radian(pitch_)),
    sin(Degree2Radian(yaw_)) * cos(Degree2Radian(pitch_))
  ).normalized();

  right_ = look_direction_.cross(Eigen::Vector3f(0, 1, 0)).normalized();
  up_ = right_.cross(look_direction_).normalized();
}

}  // namespace motion_animation