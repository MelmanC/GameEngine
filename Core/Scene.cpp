#include "Scene.hpp"
#include <fstream>
#include <memory>
#include <nlohmann/json.hpp>
#include "Cube.hpp"
#include "Gizmo.hpp"
#include "Vector3.hpp"
#include "rlgl.h"

void scene::Scene::loadFromJson(const std::string& jsonFilePath) {
  std::ifstream file(jsonFilePath);
  if (!file.is_open()) {
    throw std::runtime_error("Could not open JSON file: " + jsonFilePath);
  }
  nlohmann::json jsonData;
  file >> jsonData;
  _shapes.clear();

  for (const auto& shapeData : jsonData["shapes"]) {
    int width = shapeData["width"];
    int height = shapeData["height"];
    float depth = shapeData.value("depth", 1.0f);
    raylib::Vector3 position(shapeData["position"]["x"],
                             shapeData["position"]["y"],
                             shapeData["position"]["z"]);
    raylib::Color color(shapeData["color"]["r"], shapeData["color"]["g"],
                        shapeData["color"]["b"], shapeData["color"]["a"]);
    std::string name = shapeData.value("name", "Shape");
    std::string type = shapeData.value("type", "Unknown");
    if (type == "Cube") {
      auto cube = std::make_unique<shape::Cube>(width, height, depth, position);
      cube->setColor(color);
      cube->setName(name);
      _shapes.push_back(std::move(cube));
    }
  }
}

void scene::Scene::saveToJson(const std::string& jsonFilePath) const {
  nlohmann::json jsonData;
  jsonData["shapes"] = nlohmann::json::array();
  for (const auto& shape : _shapes) {
    nlohmann::json shapeData;
    shapeData["width"] = shape->getWidth();
    shapeData["height"] = shape->getHeight();
    shapeData["depth"] = shape->getDepth();
    shapeData["position"]["x"] = shape->getPosition().x;
    shapeData["position"]["y"] = shape->getPosition().y;
    shapeData["position"]["z"] = shape->getPosition().z;
    shapeData["color"] = {{"r", shape->getColor().r},
                          {"g", shape->getColor().g},
                          {"b", shape->getColor().b},
                          {"a", shape->getColor().a}};
    shapeData["name"] = shape->getName();
    shapeData["type"] = shape->getType();
    jsonData["shapes"].push_back(shapeData);
  }

  std::ofstream file(jsonFilePath);
  if (!file.is_open()) {
    throw std::runtime_error("Could not open JSON file for writing: " +
                             jsonFilePath);
  }
  file << jsonData.dump(2);
}

void scene::Scene::draw() const {
  for (const auto& shape : _shapes) {
    shape->draw();
  }
}

void scene::Scene::drawGizmo() const {
  auto selectedObject = getSelectedObject();
  if (selectedObject) {
    _gizmo.drawGizmo(selectedObject->getPosition());
  }
}

void scene::Scene::updateGizmo(const raylib::Camera3D& camera,
                               const raylib::Vector2& mousePos) {
  auto selectedObject = getSelectedObject();
  if (!selectedObject) {
    _gizmo.setSelectedAxis(gizmo::GizmoAxis::NONE);
    return;
  }

  if (!_isDragging) {
    gizmo::GizmoAxis hoveredAxis =
        _gizmo.getHoveredAxis(selectedObject->getPosition(), camera, mousePos);
    _gizmo.setSelectedAxis(hoveredAxis);
  }
}

void scene::Scene::handleGizmoInteraction(const raylib::Vector2& mousePos,
                                          bool isMousePressed,
                                          bool isMouseDown) {
  auto selectedObject = getSelectedObject();
  if (!selectedObject)
    return;

  if (isMousePressed && _gizmo.getSelectedAxis() != gizmo::GizmoAxis::NONE) {
    _isDragging = true;
    _lastMousePos = mousePos;
    _dragStartPos = selectedObject->getPosition();
  }

  if (_isDragging && isMouseDown) {
    raylib::Vector2 mouseDelta = Vector2Subtract(mousePos, _lastMousePos);

    float sensitivity = 0.02f;
    raylib::Vector3 movement = {0, 0, 0};

    if (_gizmo.getMode() == gizmo::GizmoType::TRANSLATE) {
      switch (_gizmo.getSelectedAxis()) {
        case gizmo::GizmoAxis::X_AXIS:
          movement.x = mouseDelta.x * sensitivity;
          break;
        case gizmo::GizmoAxis::Y_AXIS:
          movement.y = -mouseDelta.y * sensitivity;
          break;
        case gizmo::GizmoAxis::Z_AXIS:
          movement.z = -mouseDelta.x * sensitivity;
          break;
        default:
          break;
      }
    }

    raylib::Vector3 newPosition =
        Vector3Add(selectedObject->getPosition(), movement);
    selectedObject->setPosition(newPosition);
    _lastMousePos = mousePos;
  }

  if (!isMouseDown) {
    _isDragging = false;
  }
}

void scene::Scene::setSelectedObject(int index) {
  size_t nbr = static_cast<size_t>(index);
  if (index < 0 || nbr >= _shapes.size()) {
    return;
  }
  for (size_t i = 0; i < _shapes.size(); ++i) {
    if (i == nbr && !_shapes[i]->isSelected()) {
      _shapes[i]->setSelected(true);
    } else if (_shapes[i]->isSelected()) {
      _shapes[i]->setSelected(false);
    }
  }
}

shape::IGameShape* scene::Scene::getSelectedObject() const {
  for (const auto& shape : _shapes) {
    if (shape->isSelected()) {
      return shape.get();
    }
  }
  return nullptr;
}

void scene::Scene::addShape(std::unique_ptr<shape::IGameShape> shape) {
  _shapes.push_back(std::move(shape));
}
