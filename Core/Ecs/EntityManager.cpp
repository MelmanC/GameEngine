#include "EntityManager.hpp"

ecs::Entity ecs::EntityManager::createEntity() {
  EntityID entityID;
  if (!_availableEntityIDs.empty()) {
    entityID = _availableEntityIDs.front();
    _availableEntityIDs.pop();
  } else {
    entityID = _nextEntityID++;
  }
  _entities.push_back(entityID);
  return Entity(entityID);
}

void ecs::EntityManager::destroyEntity(Entity entity) {
  EntityID entityID = entity.getId();
  _availableEntityIDs.push(entityID);

  auto it = std::find(_entities.begin(), _entities.end(), entityID);
  if (it != _entities.end()) {
    _entities.erase(it);
  }
}
