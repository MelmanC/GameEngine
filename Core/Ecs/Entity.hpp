#pragma once

#include <cstdint>

using EntityID = std::uint32_t;

namespace ecs {

  class Entity {
    public:
      Entity() = default;
      explicit Entity(EntityID id) : id(id) {
      }

      EntityID getId() const {
        return id;
      }

      bool operator==(const Entity &other) const {
        return id == other.id;
      }

      bool operator!=(const Entity &other) const {
        return id != other.id;
      }

    private:
      EntityID id;
  };
}  // namespace ecs
