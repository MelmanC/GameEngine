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
                               gizmo.coneSize, gizmo.lineThickness);
          break;
        case GizmoMode::ROTATE:
          break;
        case GizmoMode::SCALE:
          break;
      }
    }
  }
}

void ecs::GizmoSystem::DrawCone(const raylib::Vector3& position,
                                const raylib::Vector3& direction, float radius,
                                float height,
                                const raylib::Color& color) const {
  raylib::Vector3 dirNorm = Vector3Normalize(direction);
  raylib::Vector3 tip = Vector3Add(position, Vector3Scale(dirNorm, height));

  DrawCylinderEx(position, tip, radius, 0.0f, 16, color);
}

void ecs::GizmoSystem::drawTranslationGizmo(const raylib::Vector3& position,
                                            GizmoAxis selectedAxis, float size,
                                            float cubeSize,
                                            float lineThickness) const {
  float axisLength = size;
  float coneHeight = axisLength * 0.5f;

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
  DrawCone(xEnd, {1, 0, 0}, cubeSize, coneHeight, xColor);

  raylib::Vector3 yEnd = Vector3Add(position, {0, axisLength, 0});
  DrawCube(Vector3Add(position, {0, axisLength / 2, 0}), lineThickness,
           axisLength, lineThickness, yColor);
  DrawCone(yEnd, {0, 1, 0}, cubeSize, coneHeight, yColor);

  raylib::Vector3 zEnd = Vector3Add(position, {0, 0, axisLength});
  DrawCube(Vector3Add(position, {0, 0, axisLength / 2}), lineThickness,
           lineThickness, axisLength, zColor);
  DrawCone(zEnd, {0, 0, 1}, cubeSize, coneHeight, zColor);
}

ecs::GizmoAxis ecs::GizmoSystem::getHoveredAxis(const raylib::Vector3& position,
                                                const raylib::Camera3D& camera,
                                                const raylib::Vector2& mousePos,
                                                float size, float coneSize,
                                                float lineThickness) const {
  float axisLength = size;
  Ray mouseRay = GetMouseRay(mousePos, camera);

  {
    raylib::Vector3 xEnd = Vector3Add(position, {axisLength, 0, 0});

    if (GetRayCollisionSphere(mouseRay, xEnd, coneSize).hit) {
      return GizmoAxis::X_AXIS;
    }

    raylib::Vector3 xline = Vector3Add(position, {axisLength / 2, 0, 0});
    BoundingBox xAxisBox = {
        Vector3Subtract(xline,
                        {axisLength / 2, lineThickness / 2, lineThickness / 2}),
        Vector3Add(xline,
                   {axisLength / 2, lineThickness / 2, lineThickness / 2})};
    if (GetRayCollisionBox(mouseRay, xAxisBox).hit) {
      return GizmoAxis::X_AXIS;
    }
  }

  {
    raylib::Vector3 yEnd = Vector3Add(position, {0, axisLength, 0});

    if (GetRayCollisionSphere(mouseRay, yEnd, coneSize).hit) {
      return GizmoAxis::Y_AXIS;
    }

    raylib::Vector3 yLine = Vector3Add(position, {0, axisLength / 2, 0});
    BoundingBox yAxisBox = {
        Vector3Subtract(yLine,
                        {lineThickness / 2, axisLength / 2, lineThickness / 2}),
        Vector3Add(yLine,
                   {lineThickness / 2, axisLength / 2, lineThickness / 2})};
    if (GetRayCollisionBox(mouseRay, yAxisBox).hit) {
      return GizmoAxis::Y_AXIS;
    }
  }

  {
    raylib::Vector3 zEnd = Vector3Add(position, {0, 0, axisLength});

    if (GetRayCollisionSphere(mouseRay, zEnd, coneSize).hit) {
      return GizmoAxis::Z_AXIS;
    }

    raylib::Vector3 zLine = Vector3Add(position, {0, 0, axisLength / 2});
    BoundingBox zAxisBox = {
        Vector3Subtract(zLine,
                        {lineThickness / 2, lineThickness / 2, axisLength / 2}),
        Vector3Add(zLine,
                   {lineThickness / 2, lineThickness / 2, axisLength / 2})};
    if (GetRayCollisionBox(mouseRay, zAxisBox).hit) {
      return GizmoAxis::Z_AXIS;
    }
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
        getHoveredAxis(position, camera, mousePos, gizmo.size, gizmo.coneSize,
                       gizmo.lineThickness);
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
