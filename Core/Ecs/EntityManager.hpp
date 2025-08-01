#pragma once

#include <queue>
#include <vector>
#include "Entity.hpp"

namespace ecs {
  class EntityManager {
    public:
      EntityManager() : _nextEntityID(1) {
      }
      ~EntityManager() = default;

      Entity createEntity();

      void destroyEntity(Entity entity);

      const std::vector<Entity> &getEntities() const {
        return _entities;
      }

    private:
      EntityID _nextEntityID;
      std::queue<EntityID> _availableEntityIDs;
      std::vector<Entity> _entities;
  };
}  // namespace ecs
