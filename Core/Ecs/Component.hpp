#pragma once

#include <cstddef>
#include "EntityManager.hpp"

namespace ecs {

  class IComponentArray {
    public:
      virtual ~IComponentArray() = default;
      virtual void entityDestroyed(Entity entityId) = 0;
  };

  template <typename T>
  class Component : public IComponentArray {
    public:
      void insertData(Entity entityId, T component) {
        if (_entityToIndexMap.find(entityId) != _entityToIndexMap.end()) {
          throw std::runtime_error(
              "Cannot insert component: Entity already has a component.");
        }
        size_t index = _size;
        _entityToIndexMap[entityId] = index;
        _indexToEntityMap[index] = entityId;
        _componentArray[index] = component;
        _size++;
      }

      void removeData(Entity entityId) {
        if (_entityToIndexMap.find(entityId) == _entityToIndexMap.end()) {
          throw std::runtime_error(
              "Cannot remove component: Entity does not have a component.");
        }
        size_t indexRemoved = _entityToIndexMap[entityId];
        size_t lastIndex = _size - 1;
        _componentArray[indexRemoved] = _componentArray[lastIndex];

        Entity lastEntity = _indexToEntityMap[lastIndex];
        _entityToIndexMap[lastEntity] = indexRemoved;
        _indexToEntityMap[indexRemoved] = lastEntity;

        _entityToIndexMap.erase(entityId);
        _indexToEntityMap.erase(lastIndex);
        --_size;
      }

      T &getData(Entity entityId) {
        if (_entityToIndexMap.find(entityId) == _entityToIndexMap.end()) {
          throw std::runtime_error(
              "Cannot get component: Entity does not have a component.");
        }
        return _componentArray[_entityToIndexMap[entityId]];
      }

      void entityDestroyed(Entity entityId) override {
        if (_entityToIndexMap.find(entityId) == _entityToIndexMap.end()) {
          throw std::runtime_error(
              "Cannot destroy entity: Entity does not have a component.");
        }
        removeData(entityId);
      }

    private:
      std::array<T, MAX_ENTITIES> _componentArray;

      std::unordered_map<Entity, size_t> _entityToIndexMap;

      std::unordered_map<size_t, Entity> _indexToEntityMap;

      size_t _size;
  };
}  // namespace ecs
