#include "Scene.hpp"
#include <memory>
#include <nlohmann/json.hpp>
#include "Gizmo.hpp"
#include "rlgl.h"

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
  _gizmo.update(getSelectedObject(), camera, mousePos);
}

void scene::Scene::handleGizmoInteraction(const raylib::Vector2& mousePos,
                                          bool isMousePressed,
                                          bool isMouseDown) {
  _gizmo.handleInteraction(getSelectedObject(), mousePos, isMousePressed,
                           isMouseDown);
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
