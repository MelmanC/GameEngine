#include "GizmoSystem.hpp"
#include <raylib.h>
#include "ECSManager.hpp"
#include "GizmoComponent.hpp"
#include "SelectionComponent.hpp"
#include "TransformHelper.hpp"

void ecs::GizmoSystem::update(float deltaTime) {
  (void)deltaTime;
}

void ecs::GizmoSystem::drawGizmos() {
  if (!_ecsManager)
    return;

  for (Entity entity : _entities) {
    if (!_ecsManager->hasComponent<GizmoComponent>(entity) ||
        !_ecsManager->hasComponent<SelectionComponent>(entity)) {
      continue;
    }
    auto& gizmo = _ecsManager->getComponent<GizmoComponent>(entity);
    auto& selection = _ecsManager->getComponent<SelectionComponent>(entity);

    if (selection.selected) {
      raylib::Vector3 position =
          TransformHelper::getPosition(entity, _ecsManager);

      switch (gizmo.mode) {
        case GizmoMode::TRANSLATE:
          drawTranslationGizmo(position, gizmo.selectedAxis, gizmo.size,
                               gizmo.cubeSize, gizmo.lineThickness);
          break;
        case GizmoMode::ROTATE:
          break;
        case GizmoMode::SCALE:
          break;
      }
    }
  }
}

void ecs::GizmoSystem::drawTranslationGizmo(const raylib::Vector3& position,
                                            GizmoAxis selectedAxis, float size,
                                            float cubeSize,
                                            float lineThickness) const {
  float axisLength = size;

  raylib::Color xColor = (selectedAxis == GizmoAxis::X_AXIS)
                             ? raylib::Color::Yellow()
                             : raylib::Color::Red();
  raylib::Color yColor = (selectedAxis == GizmoAxis::Y_AXIS)
                             ? raylib::Color::Yellow()
                             : raylib::Color::Green();
  raylib::Color zColor = (selectedAxis == GizmoAxis::Z_AXIS)
                             ? raylib::Color::Yellow()
                             : raylib::Color::Blue();

  raylib::Vector3 xEnd = Vector3Add(position, {axisLength, 0, 0});
  DrawCube(Vector3Add(position, {axisLength / 2, 0, 0}), axisLength,
           lineThickness, lineThickness, xColor);
  DrawCube(xEnd, cubeSize, cubeSize, cubeSize, xColor);

  raylib::Vector3 yEnd = Vector3Add(position, {0, axisLength, 0});
  DrawCube(Vector3Add(position, {0, axisLength / 2, 0}), lineThickness,
           axisLength, lineThickness, yColor);
  DrawCube(yEnd, cubeSize, cubeSize, cubeSize, yColor);

  raylib::Vector3 zEnd = Vector3Add(position, {0, 0, axisLength});
  DrawCube(Vector3Add(position, {0, 0, axisLength / 2}), lineThickness,
           lineThickness, axisLength, zColor);
  DrawCube(zEnd, cubeSize, cubeSize, cubeSize, zColor);
}

ecs::GizmoAxis ecs::GizmoSystem::getHoveredAxis(const raylib::Vector3& position,
                                                const raylib::Camera3D& camera,
                                                const raylib::Vector2& mousePos,
                                                float size,
                                                float cubeSize) const {
  float axisLength = size;
  Ray mouseRay = GetMouseRay(mousePos, camera);

  raylib::Vector3 xCubePos = Vector3Add(position, {axisLength, 0, 0});
  BoundingBox xBox = {
      Vector3Subtract(xCubePos, {cubeSize / 2, cubeSize / 2, cubeSize / 2}),
      Vector3Add(xCubePos, {cubeSize / 2, cubeSize / 2, cubeSize / 2})};
  if (GetRayCollisionBox(mouseRay, xBox).hit) {
    return GizmoAxis::X_AXIS;
  }

  raylib::Vector3 yCubePos = Vector3Add(position, {0, axisLength, 0});
  BoundingBox yBox = {
      Vector3Subtract(yCubePos, {cubeSize / 2, cubeSize / 2, cubeSize / 2}),
      Vector3Add(yCubePos, {cubeSize / 2, cubeSize / 2, cubeSize / 2})};
  if (GetRayCollisionBox(mouseRay, yBox).hit) {
    return GizmoAxis::Y_AXIS;
  }

  raylib::Vector3 zCubePos = Vector3Add(position, {0, 0, axisLength});
  BoundingBox zBox = {
      Vector3Subtract(zCubePos, {cubeSize / 2, cubeSize / 2, cubeSize / 2}),
      Vector3Add(zCubePos, {cubeSize / 2, cubeSize / 2, cubeSize / 2})};
  if (GetRayCollisionBox(mouseRay, zBox).hit) {
    return GizmoAxis::Z_AXIS;
  }

  return GizmoAxis::NONE;
}

void ecs::GizmoSystem::updateGizmo(Entity selectedEntity,
                                   const raylib::Camera3D& camera,
                                   const raylib::Vector2& mousePos) {
  if (!_ecsManager)
    return;

  auto& gizmo = _ecsManager->getComponent<GizmoComponent>(selectedEntity);
  raylib::Vector3 position =
      TransformHelper::getPosition(selectedEntity, _ecsManager);

  if (!gizmo.isDragging) {
    GizmoAxis hoveredAxis =
        getHoveredAxis(position, camera, mousePos, gizmo.size, gizmo.cubeSize);
    gizmo.selectedAxis = hoveredAxis;
  }
}

void ecs::GizmoSystem::handleGizmoInteraction(Entity selectedEntity,
                                              const raylib::Vector2& mousePos,
                                              bool isMousePressed,
                                              bool isMouseDown) {
  if (!_ecsManager)
    return;

  auto& gizmo = _ecsManager->getComponent<GizmoComponent>(selectedEntity);

  if (isMousePressed && gizmo.selectedAxis != GizmoAxis::NONE) {
    gizmo.isDragging = true;
    gizmo.lastMousePos = mousePos;
    gizmo.dragStartPos =
        TransformHelper::getPosition(selectedEntity, _ecsManager);
  }

  if (gizmo.isDragging && isMouseDown) {
    raylib::Vector2 mouseDelta = Vector2Subtract(mousePos, gizmo.lastMousePos);
    float sensitivity = 0.02f;
    raylib::Vector3 movement = {0, 0, 0};

    if (gizmo.mode == GizmoMode::TRANSLATE) {
      switch (gizmo.selectedAxis) {
        case GizmoAxis::X_AXIS:
          movement.x = mouseDelta.x * sensitivity;
          break;
        case GizmoAxis::Y_AXIS:
          movement.y = -mouseDelta.y * sensitivity;
          break;
        case GizmoAxis::Z_AXIS:
          movement.z = -mouseDelta.x * sensitivity;
          break;
        default:
          break;
      }
    }

    TransformHelper::movePosition(selectedEntity, movement, _ecsManager);
    gizmo.lastMousePos = mousePos;
  }

  if (!isMouseDown) {
    gizmo.isDragging = false;
  }
}
