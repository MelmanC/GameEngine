#pragma once

#include <bitset>
#include <cstdint>
#include <queue>

constexpr std::size_t MAX_ENTITIES = 3000;
using ComponentType = std::uint8_t;
constexpr ComponentType MAX_COMPONENTS = 32;

using Entity = std::uint32_t;

using Signature = std::bitset<MAX_COMPONENTS>;

namespace ecs {

  class EntityManager {
    public:
      EntityManager();

      ~EntityManager() = default;

      Entity createEntity();
      void destroyEntity(Entity entityId);

      void setSignature(Entity entityId, Signature signature);
      Signature getSignature(Entity entityId) const;

      std::vector<Entity> getAllLivingEntities() const;

      int getEntityCount() const {
        return _livingEntityCount;
      }

    private:
      std::queue<Entity> _poolEntitiesIDs;

      std::array<Signature, MAX_ENTITIES> _signatures;

      uint32_t _livingEntityCount = 0;
  };
}  // namespace ecs
