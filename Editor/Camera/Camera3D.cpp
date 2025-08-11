#include "Camera3D.hpp"

camera::Camera3D::Camera3D() {
  _camera = raylib::Camera();
  _camera.SetPosition(raylib::Vector3(0.0f, 10.0f, 10.0f));
  _camera.SetTarget(raylib::Vector3(0.0f, 0.0f, 0.0f));
  _camera.SetUp(raylib::Vector3(0.0f, 1.0f, 0.0f));
  _camera.SetFovy(45.0f);
  _camera.SetProjection(CAMERA_PERSPECTIVE);
}

void camera::Camera3D::update(bool isViewportActive) {
  if (isViewportActive) {
    handleMouseLook();
  }
}

void camera::Camera3D::handleMouseLook() {
  if (IsCursorHidden()) {
    float mouseSensitivity = 0.003f;
    raylib::Vector2 mouseMovement = GetMouseDelta();

    raylib::Vector3 forward = Vector3Subtract(_camera.target, _camera.position);
    forward = Vector3Normalize(forward);

    raylib::Vector3 right = Vector3CrossProduct(forward, _camera.up);
    right = Vector3Normalize(right);

    float yaw = -mouseMovement.x * mouseSensitivity;
    forward = Vector3RotateByAxisAngle(forward, _camera.up, yaw);

    float pitch = -mouseMovement.y * mouseSensitivity;
    forward = Vector3RotateByAxisAngle(forward, right, pitch);

    _camera.target = Vector3Add(_camera.position, forward);
  }
}

void camera::Camera3D::handleMovementInput(int forwardKey, int backwardKey,
                                           int leftKey, int rightKey) {
  if (!IsCursorHidden())
    return;

  raylib::Vector3 forward = Vector3Subtract(_camera.target, _camera.position);
  forward = Vector3Normalize(forward);
  raylib::Vector3 right = Vector3CrossProduct(forward, _camera.up);
  right = Vector3Normalize(right);

  float speed = _speed * GetFrameTime();

  if (IsKeyDown(forwardKey)) {
    _camera.position =
        Vector3Add(_camera.position, Vector3Scale(forward, speed));
    _camera.target = Vector3Add(_camera.target, Vector3Scale(forward, speed));
  }
  if (IsKeyDown(backwardKey)) {
    _camera.position =
        Vector3Subtract(_camera.position, Vector3Scale(forward, speed));
    _camera.target =
        Vector3Subtract(_camera.target, Vector3Scale(forward, speed));
  }
  if (IsKeyDown(leftKey)) {
    _camera.position =
        Vector3Subtract(_camera.position, Vector3Scale(right, speed));
    _camera.target =
        Vector3Subtract(_camera.target, Vector3Scale(right, speed));
  }
  if (IsKeyDown(rightKey)) {
    _camera.position = Vector3Add(_camera.position, Vector3Scale(right, speed));
    _camera.target = Vector3Add(_camera.target, Vector3Scale(right, speed));
  }

  if (IsKeyDown(KEY_SPACE)) {
    _camera.position.y += speed;
    _camera.target.y += speed;
  }
  if (IsKeyDown(KEY_LEFT_SHIFT)) {
    _camera.position.y -= speed;
    _camera.target.y -= speed;
  }
}
