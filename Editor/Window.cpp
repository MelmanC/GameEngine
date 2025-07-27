#include "Window.hpp"
#include <raylib.hpp>
#include "raygui.h"

gui::Window::Window(int width, int height, const char *title)
    : _width(width), _height(height), _title(title), _isRunning(true) {
  _window.Init(_width, _height, _title);
  _camera.position = raylib::Vector3(10.0f, 10.0f, 10.0f);
  _camera.target = raylib::Vector3(0.0f, 0.0f, 0.0f);
  _camera.up = raylib::Vector3(0.0f, 1.0f, 0.0f);
  _camera.fovy = 45.0f;
  _camera.projection = CAMERA_PERSPECTIVE;

  _scene.loadFromJson("./scene.json");
}

gui::Window::~Window() {
  if (_isRunning) {
    EnableCursor();
    _window.Close();
  }
}

void gui::Window::run() {
  while (_isRunning && !_window.ShouldClose()) {
    handleInput();
    updateCamera();
    draw();
  }
}

void gui::Window::draw() {
  _window.BeginDrawing();

  raylib::Color backgroundColor(200, 200, 200);
  raylib::Color textColor(0, 0, 0);

  _window.ClearBackground(backgroundColor);

  drawCamera();

  _window.EndDrawing();
}

void gui::Window::drawCamera() {
  _camera.BeginMode();

  DrawGrid(20, 1.0f);

  _scene.draw();

  _camera.EndMode();
}

void gui::Window::updateCamera() {
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

    float speed = 10.0f * GetFrameTime();

    if (IsKeyDown(KEY_W)) {
      _camera.position =
          Vector3Add(_camera.position, Vector3Scale(forward, speed));
      _camera.target = Vector3Add(_camera.target, Vector3Scale(forward, speed));
    }
    if (IsKeyDown(KEY_S)) {
      _camera.position =
          Vector3Subtract(_camera.position, Vector3Scale(forward, speed));
      _camera.target =
          Vector3Subtract(_camera.target, Vector3Scale(forward, speed));
    }
    if (IsKeyDown(KEY_A)) {
      _camera.position =
          Vector3Subtract(_camera.position, Vector3Scale(right, speed));
      _camera.target =
          Vector3Subtract(_camera.target, Vector3Scale(right, speed));
    }
    if (IsKeyDown(KEY_D)) {
      _camera.position =
          Vector3Add(_camera.position, Vector3Scale(right, speed));
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
}

void gui::Window::handleInput() {
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !IsCursorHidden()) {
    DisableCursor();
  }
  if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && IsCursorHidden()) {
    EnableCursor();
  }
}
