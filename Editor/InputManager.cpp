#include "InputManager.hpp"
#include <imgui.h>
#include <raylib.h>
#include <iostream>
#include "Application.hpp"
#include "ECSManager.hpp"
#include "GizmoComponent.hpp"
#include "RayCollision.hpp"
#include "RenderComponent.hpp"
#include "TransformComponent.hpp"

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

void input::InputManager::handleEntitySelection() {
  if (!canProcessMouse() || _app.isViewportActive())
    return;

  Ray ray = GetMouseRay(GetMousePosition(), _app.getCamera().getCamera());
  auto& ecsManager = _app.getECSManager();
  auto& selectionSystem = _app.getSelectionSystem();

  Entity selectedEntity = 0;
  bool entityFound = false;

  std::vector<Entity> entities = _app.getScene().getAllEntities();

  for (Entity entity : entities) {
    try {
      auto& transform =
          ecsManager.getComponent<ecs::TransformComponent>(entity);
      auto& render = ecsManager.getComponent<ecs::RenderComponent>(entity);

      if (!render.visible)
        continue;

      BoundingBox box = transform.getBoundingBox();
      raylib::RayCollision collision = GetRayCollisionBox(ray, box);

      if (collision.hit) {
        selectedEntity = entity;
        entityFound = true;
        break;
      }
    } catch (const std::exception& e) {
      continue;
    }
  }

  if (entityFound) {
    _app.getScene().setSelectedEntity(selectedEntity);
  } else {
    selectionSystem.deselectAll();
  }
}

void input::InputManager::handleMouseInput() {
  if (!canProcessMouse())
    return;

  raylib::Vector2 mousePos = GetMousePosition();
  bool isMousePressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
  bool isMouseDown = IsMouseButtonDown(MOUSE_BUTTON_LEFT);

  bool gizmoInteraction = false;
  if (_app.getSelectionSystem().hasSelection()) {
    Entity selectedEntity = _app.getSelectionSystem().getSelectedEntity();

    if (isMouseInViewport(mousePos)) {
      auto& gizmo = _app.getECSManager().getComponent<ecs::GizmoComponent>(
          selectedEntity);
      auto& transform =
          _app.getECSManager().getComponent<ecs::TransformComponent>(
              selectedEntity);

      ecs::GizmoAxis hoveredAxis = _app.getGizmoSystem().getHoveredAxis(
          transform.position, _app.getCamera().getCamera(), mousePos,
          gizmo.size, gizmo.cubeSize);

      if (hoveredAxis != ecs::GizmoAxis::NONE || gizmo.isDragging) {
        gizmoInteraction = true;
      }
    }
  }

  _app.getViewportManager().handleGizmoInteraction(
      mousePos, isMousePressed, isMouseDown, &_app.getECSManager(),
      &_app.getGizmoSystem(), &_app.getSelectionSystem());

  if (!isMouseDown) {
    _app.getViewportManager().updateGizmo(
        _app.getCamera(), mousePos, &_app.getECSManager(),
        &_app.getGizmoSystem(), &_app.getSelectionSystem());
  }

  if (isMousePressed && !gizmoInteraction)
    handleEntitySelection();
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
      _app.getScene().clearEntities();
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

bool input::InputManager::isMouseInViewport(
    const raylib::Vector2& mousePos) const {
  return _app.getViewportManager().isMouseInViewport(mousePos);
}
