#include "Scene.hpp"
#include <memory>
#include <nlohmann/json.hpp>
#include "EntityManager.hpp"
#include "JsonHandler.hpp"
#include "SelectionSystem.hpp"
#include "rlgl.h"

void scene::Scene::initialize(ecs::SelectionSystem* selectionSystem,
                              ecs::ECSManager* ecsManager) {
  _selectionSystem = selectionSystem;
  _ecsManager = ecsManager;
  _jsonHandler = std::make_unique<jsonfile::JsonHandler>();
}

void scene::Scene::setSelectedEntity(Entity entity) {
  if (_selectionSystem && _ecsManager) {
    _selectionSystem->selectEntity(entity);
  }
}

Entity scene::Scene::getSelectedEntity() const {
  if (_selectionSystem) {
    return _selectionSystem->getSelectedEntity();
  }
  return 0;
}

void scene::Scene::clearEntities() {
  if (!_ecsManager)
    return;
  for (const auto& entity : _ecsManager->getAllEntities()) {
    _ecsManager->destroyEntity(entity);
  }
}

std::vector<Entity> scene::Scene::getAllEntities() const {
  if (_ecsManager) {
    return _ecsManager->getAllEntities();
  }
  return {};
}

void scene::Scene::save(const std::string& jsonFilePath) const {
  if (_jsonHandler && _ecsManager) {
    _jsonHandler->saveToJson(*this, jsonFilePath, _ecsManager);
  }
}

bool scene::Scene::hasSelectedEntity() const {
  if (_selectionSystem) {
    return _selectionSystem->hasSelection();
  }
  return false;
}

void scene::Scene::load(const std::string& jsonFilePath) {
  if (_jsonHandler && _ecsManager) {
    _jsonHandler->loadFromJson(*this, jsonFilePath, _ecsManager);
  }
}
