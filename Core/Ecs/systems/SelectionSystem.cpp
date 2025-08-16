#include "SelectionSystem.hpp"
#include "ECSManager.hpp"
#include "SelectionComponent.hpp"

void ecs::SelectionSystem::update(__attribute__((unused)) float deltaTime) {
}

void ecs::SelectionSystem::selectEntity(Entity entity) {
  deselectAll();

  auto& selection = _ecsManager->getComponent<SelectionComponent>(entity);
  selection.selected = true;
  _selectedEntity = entity;
  _hasSelection = true;
}

void ecs::SelectionSystem::deselectAll() {
  for (Entity entity : _entities) {
    auto& selection = _ecsManager->getComponent<SelectionComponent>(entity);
    selection.selected = false;
  }
  _hasSelection = false;
}

Entity ecs::SelectionSystem::getSelectedEntity() const {
  return _selectedEntity;
}

bool ecs::SelectionSystem::hasSelection() const {
  return _hasSelection;
}
