#include "Gizmo.hpp"
#include <raylib.h>

void gizmo::Gizmo::drawGizmo(const raylib::Vector3& position) const {
  switch (_mode) {
    case GizmoType::TRANSLATE:
      drawTranslationGizmo(position);
      break;
    case GizmoType::ROTATE:
      // drawRotationGizmo(position);
      break;
    case GizmoType::SCALE:
      // drawScaleGizmo(position);
      break;
  }
}

void gizmo::Gizmo::drawTranslationGizmo(const raylib::Vector3& position) const {
  float axisLength = _size;

  raylib::Color xColor = (_selectedAxis == GizmoAxis::X_AXIS)
                             ? raylib::Color::Yellow()
                             : raylib::Color::Red();
  raylib::Color yColor = (_selectedAxis == GizmoAxis::Y_AXIS)
                             ? raylib::Color::Yellow()
                             : raylib::Color::Green();
  raylib::Color zColor = (_selectedAxis == GizmoAxis::Z_AXIS)
                             ? raylib::Color::Yellow()
                             : raylib::Color::Blue();

  raylib::Vector3 xEnd = Vector3Add(position, {axisLength, 0, 0});
  DrawCube(Vector3Add(position, {axisLength / 2, 0, 0}), axisLength,
           _lineThickness, _lineThickness, xColor);
  DrawCube(xEnd, _cubeSize, _cubeSize, _cubeSize, xColor);

  raylib::Vector3 yEnd = Vector3Add(position, {0, axisLength, 0});
  DrawCube(Vector3Add(position, {0, axisLength / 2, 0}), _lineThickness,
           axisLength, _lineThickness, yColor);
  DrawCube(yEnd, _cubeSize, _cubeSize, _cubeSize, yColor);

  raylib::Vector3 zEnd = Vector3Add(position, {0, 0, axisLength});
  DrawCube(Vector3Add(position, {0, 0, axisLength / 2}), _lineThickness,
           _lineThickness, axisLength, zColor);
  DrawCube(zEnd, _cubeSize, _cubeSize, _cubeSize, zColor);
}

gizmo::GizmoAxis gizmo::Gizmo::getHoveredAxis(
    const raylib::Vector3& position, const raylib::Camera3D& camera,
    const raylib::Vector2& mousePos) const {
  float axisLength = _size;

  Ray mouseRay = GetMouseRay(mousePos, camera);

  raylib::Vector3 xCubePos = Vector3Add(position, {axisLength, 0, 0});
  BoundingBox xBox = {
      Vector3Subtract(xCubePos, {_cubeSize / 2, _cubeSize / 2, _cubeSize / 2}),
      Vector3Add(xCubePos, {_cubeSize / 2, _cubeSize / 2, _cubeSize / 2})};
  if (GetRayCollisionBox(mouseRay, xBox).hit) {
    return GizmoAxis::X_AXIS;
  }

  raylib::Vector3 yCubePos = Vector3Add(position, {0, axisLength, 0});
  BoundingBox yBox = {
      Vector3Subtract(yCubePos, {_cubeSize / 2, _cubeSize / 2, _cubeSize / 2}),
      Vector3Add(yCubePos, {_cubeSize / 2, _cubeSize / 2, _cubeSize / 2})};
  if (GetRayCollisionBox(mouseRay, yBox).hit) {
    return GizmoAxis::Y_AXIS;
  }

  raylib::Vector3 zCubePos = Vector3Add(position, {0, 0, axisLength});
  BoundingBox zBox = {
      Vector3Subtract(zCubePos, {_cubeSize / 2, _cubeSize / 2, _cubeSize / 2}),
      Vector3Add(zCubePos, {_cubeSize / 2, _cubeSize / 2, _cubeSize / 2})};
  if (GetRayCollisionBox(mouseRay, zBox).hit) {
    return GizmoAxis::Z_AXIS;
  }
  return GizmoAxis::NONE;
}
