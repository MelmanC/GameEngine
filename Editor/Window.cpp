#include "Window.hpp"
#include <raylib.h>
#include <raylib.hpp>
#define RAYGUI_IMPLEMENTATION
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#include "raygui.h"
#pragma GCC diagnostic pop

gui::Window::Window(int width, int height, const char *title)
    : _width(width), _height(height), _title(title), _isRunning(true) {
  _window.Init(_width, _height, _title);
  _window.SetTargetFPS(60);

  _toolbarPanel = raylib::Rectangle(0, 0, _width, 40);
  _hierarchyPanel = raylib::Rectangle(0, 40, 250, _height - 40);
  _propertiesPanel = raylib::Rectangle(_width - 300, 40, 300, _height - 40);
  _viewport = raylib::Rectangle(250, 40, _width - 550, _height - 30);

  // Initialize the camera
  _camera.position = raylib::Vector3(10.0f, 10.0f, 10.0f);
  _camera.target = raylib::Vector3(0.0f, 0.0f, 0.0f);
  _camera.up = raylib::Vector3(0.0f, 1.0f, 0.0f);
  _camera.fovy = 45.0f;
  _camera.projection = CAMERA_PERSPECTIVE;

  _scene.loadFromJson("./scene.json");
}

gui::Window::~Window() {
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

  drawInterface();

  _window.EndDrawing();
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

void gui::Window::drawViewport() {
  BeginScissorMode(_viewport.x, _viewport.y, _viewport.width, _viewport.height);
  _camera.BeginMode();
  DrawGrid(20, 1.0f);
  _scene.draw();
  _camera.EndMode();
  EndScissorMode();

  if (_isViewportActive) {
    DrawRectangleLinesEx(_viewport, 2, raylib::Color::Blue());
  } else {
    DrawRectangleLinesEx(_viewport, 1, raylib::Color::Gray());
  }
}

void gui::Window::drawInterface() {
  // Draw the toolbar panel
  GuiPanel(_toolbarPanel, "Toolbar");
  if (GuiButton({_toolbarPanel.x + 10, _toolbarPanel.y + 8, 60, 24}, "New")) {
  }
  if (GuiButton({_toolbarPanel.x + 80, _toolbarPanel.y + 8, 60, 24}, "Load")) {
  }
  if (GuiButton({_toolbarPanel.x + 150, _toolbarPanel.y + 8, 60, 24}, "Save")) {
    _scene.saveToJson("./scene.json");
  }

  // Draw the hierarchy panel
  GuiPanel(_hierarchyPanel, "Hierarchy");
  float yOffset = _hierarchyPanel.y + 30;
  for (size_t i = 0; i < _scene._shapes.size(); ++i) {
    const shape::GameShape &shape = _scene._shapes[i];
    std::string objectName =
        "Object " + std::to_string(i + 1) + ": " + shape.getName();

    if (GuiButton(
            {_hierarchyPanel.x + 10, yOffset, _hierarchyPanel.width - 20, 25},
            objectName.c_str())) {
      _scene.setSelectedObject(i);
    }
    yOffset += 30;
  }
  if (GuiButton({_hierarchyPanel.x + 10, yOffset + 10,
                 _hierarchyPanel.width - 20, 25},
                "Add Cube")) {
    _scene._shapes.push_back(
        shape::GameShape(2.0f, 2.0f, 2.0f, raylib::Vector3(10, 0, 0)));
    _scene.saveToJson("./scene.json");
  }

  // Draw the properties panel
  GuiPanel(_propertiesPanel, "Properties");
  GuiLabel({_propertiesPanel.x + 10, _propertiesPanel.y + 30, 100, 20},
           "Camera Info:");
  std::string posText = "Pos: " + std::to_string(_camera.position.x) + ", " +
                        std::to_string(_camera.position.y) + ", " +
                        std::to_string(_camera.position.z);
  GuiLabel({_propertiesPanel.x + 10, _propertiesPanel.y + 55, 200, 20},
           posText.c_str());
  std::string targetText = "Target: " + std::to_string(_camera.target.x) +
                           ", " + std::to_string(_camera.target.y) + ", " +
                           std::to_string(_camera.target.z);
  GuiLabel({_propertiesPanel.x + 10, _propertiesPanel.y + 80, 200, 20},
           targetText.c_str());
  GuiLabel({_propertiesPanel.x + 10, _propertiesPanel.y + 110, 100, 20},
           "Viewport:");
  std::string viewportText = "Size: " + std::to_string(_viewport.width) + "x" +
                             std::to_string(_viewport.height);
  GuiLabel({_propertiesPanel.x + 10, _propertiesPanel.y + 135, 200, 20},
           viewportText.c_str());
}
