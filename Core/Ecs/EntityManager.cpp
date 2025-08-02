#include "EntityManager.hpp"

ecs::EntityManager::EntityManager() {
  for (Entity entity = 0; entity < MAX_ENTITIES; ++entity) {
    _poolEntitiesIDs.push(entity);
  }
}

Entity ecs::EntityManager::createEntity() {
  if (_poolEntitiesIDs.empty())
    throw std::runtime_error("No more entities available");

  Entity entityId = _poolEntitiesIDs.front();
  _poolEntitiesIDs.pop();
  ++_livingEntityCount;
  return entityId;
}

void ecs::EntityManager::destroyEntity(Entity entityId) {
  if (entityId >= MAX_ENTITIES)
    throw std::out_of_range("Entity ID out of range");

  _signatures[entityId].reset();
  _poolEntitiesIDs.push(entityId);
  --_livingEntityCount;
}

void ecs::EntityManager::setSignature(Entity entityId, Signature signature) {
  if (entityId >= MAX_ENTITIES)
    throw std::out_of_range("Entity ID out of range");

  _signatures[entityId] = signature;
}

Signature ecs::EntityManager::getSignature(Entity entityId) const {
  if (entityId >= MAX_ENTITIES)
    throw std::out_of_range("Entity ID out of range");

  return _signatures[entityId];
}
