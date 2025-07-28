#include "Window.hpp"
#include <imgui.h>
#include <raylib.h>
#include <rlImGui.h>
#include <raylib.hpp>

gui::Window::Window(int width, int height, const char *title)
    : _width(width),
      _height(height),
      _title(title),
      _isRunning(true),
      _isViewportActive(false),
      _gui(width, height, *this) {
  _window.Init(_width, _height, _title);
  _window.SetTargetFPS(60);

  rlImGuiSetup(true);

  _viewport = raylib::Rectangle(250, 40, _width - 550, _height - 30);

  _camera.position = raylib::Vector3(10.0f, 10.0f, 10.0f);
  _camera.target = raylib::Vector3(0.0f, 0.0f, 0.0f);
  _camera.up = raylib::Vector3(0.0f, 1.0f, 0.0f);
  _camera.fovy = 45.0f;
  _camera.projection = CAMERA_PERSPECTIVE;

  _scene.loadFromJson("./scene.json");
}

gui::Window::~Window() {
  rlImGuiShutdown();
  if (_isRunning) {
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

bool gui::Window::isMouseInViewport() {
  raylib::Vector2 mousePosition = GetMousePosition();
  return CheckCollisionPointRec(mousePosition, _viewport);
}

void gui::Window::draw() {
  _window.BeginDrawing();

  raylib::Color backgroundColor(45, 45, 48);
  _window.ClearBackground(backgroundColor);

  DrawRectangleRec(_viewport, raylib::Color(135, 206, 235));

  drawViewport();

  rlImGuiBegin();

  _gui.drawInterface();

  rlImGuiEnd();

  _window.EndDrawing();
}

void gui::Window::updateCamera() {
  if (!ImGui::GetIO().WantCaptureMouse && _isViewportActive &&
      IsCursorHidden()) {
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
  if (!ImGui::GetIO().WantCaptureKeyboard) {
    if (IsKeyPressed(KEY_ESCAPE)) {
      _isRunning = false;
    }

    if (IsKeyPressed(KEY_F)) {
      _isViewportActive = !_isViewportActive;
      if (_isViewportActive) {
        SetMousePosition(_viewport.x + _viewport.width / 2,
                         _viewport.y + _viewport.height / 2);
        DisableCursor();
      } else {
        EnableCursor();
      }
    }
  }
}

void gui::Window::drawViewport() {
  BeginScissorMode(_viewport.x, _viewport.y, _viewport.width, _viewport.height);
  _camera.BeginMode();
  DrawGrid(20, 1.0f);

  _scene.draw();

  _camera.EndMode();

  _camera.BeginMode();

  rlDrawRenderBatchActive();
  rlDisableDepthTest();

  _scene.drawGizmo();

  rlDrawRenderBatchActive();
  rlEnableDepthTest();

  _camera.EndMode();

  EndScissorMode();

  if (isMouseInViewport() && !_isViewportActive) {
    raylib::Vector2 mousePos = GetMousePosition();
    bool isMousePressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    bool isMouseDown = IsMouseButtonDown(MOUSE_BUTTON_LEFT);

    _scene.handleGizmoInteraction(mousePos, isMousePressed, isMouseDown);
    if (!isMouseDown) {
      _scene.updateGizmo(_camera, mousePos);
    }
  }

  if (_isViewportActive) {
    DrawRectangleLinesEx(_viewport, 2, raylib::Color::Red());
  } else {
    DrawRectangleLinesEx(_viewport, 1, raylib::Color::Gray());
  }
}
