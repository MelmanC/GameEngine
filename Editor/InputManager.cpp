#include "InputManager.hpp"
#include <imgui.h>
#include <raylib.h>
#include <iostream>
#include "Application.hpp"
#include "RayCollision.hpp"

input::InputManager::InputManager(app::Application& app) : _app(app) {
}

void input::InputManager::handleInput() {
  handleKeyboardInput();
  handleMouseInput();
}

void input::InputManager::handleKeyboardInput() {
  if (!canProcessKeyboard())
    return;

  handleApplicationShortcuts();
  handleCameraMovement();
}

void input::InputManager::handleObjectSelection() {
  if (!canProcessMouse())
    return;
  Ray ray = GetMouseRay(GetMousePosition(), _app.getCamera().getCamera());
  const auto& shapes = _app.getScene().getShapes();
  int selectedIndex = -1;
  for (size_t i = 0; i < shapes.size(); ++i) {
    BoundingBox box = shapes[i]->getBoundingBox();
    raylib::RayCollision collision = GetRayCollisionBox(ray, box);

    if (collision.hit) {
      selectedIndex = i;
      break;
    }
  }
  if (selectedIndex != -1) {
    _app.getScene().setSelectedObject(selectedIndex);
  } else {
    _app.getScene().setSelectedObject(selectedIndex);
  }
}

void input::InputManager::handleMouseInput() {
  if (!canProcessMouse())
    return;

  raylib::Vector2 mousePos = GetMousePosition();
  bool isMousePressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
  bool isMouseDown = IsMouseButtonDown(MOUSE_BUTTON_LEFT);

  _app.getViewportManager().handleGizmoInteraction(_app.getScene(), mousePos,
                                                   isMousePressed, isMouseDown);

  if (!isMouseDown) {
    _app.getViewportManager().updateGizmo(_app.getScene(), _app.getCamera(),
                                          mousePos);
  }
  if (isMousePressed)
    handleObjectSelection();
}

void input::InputManager::handleApplicationShortcuts() {
  if (IsKeyPressed(KEY_ESCAPE)) {
    _app.setRunning(false);
    return;
  }

  if (IsKeyPressed(KEY_F)) {
    _app.setViewportActive(!_app.isViewportActive());
    return;
  }

  bool ctrlPressed =
      IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL);
  if (ctrlPressed) {
    if (IsKeyPressed(KEY_S)) {
      try {
        _app.getScene().save("./scene.json");
        std::cout << "Scene saved!" << std::endl;
      } catch (const std::exception& e) {
        std::cout << "Error saving: " << e.what() << std::endl;
      }
    } else if (IsKeyPressed(KEY_N)) {
      _app.getScene().clearShapes();
      std::cout << "New scene created!" << std::endl;
    } else if (IsKeyPressed(KEY_O)) {
      try {
        _app.getScene().load("./scene.json");
        std::cout << "Scene loaded!" << std::endl;
      } catch (const std::exception& e) {
        std::cout << "Error loading: " << e.what() << std::endl;
      }
    }
  }
}

void input::InputManager::handleCameraMovement() {
  if (!_app.isViewportActive())
    return;

  int forwardKey = KEY_W;
  int leftKey = KEY_A;
  int backwardKey = KEY_S;
  int rightKey = KEY_D;
  _app.getCamera().handleMovementInput(forwardKey, backwardKey, leftKey,
                                       rightKey);
}

bool input::InputManager::canProcessKeyboard() const {
  return !ImGui::GetIO().WantCaptureKeyboard;
}

bool input::InputManager::canProcessMouse() const {
  return !ImGui::GetIO().WantCaptureMouse;
}
