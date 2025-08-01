#pragma once

namespace ecs {

  using EntityID = unsigned int;

  class Entity {
    public:
      Entity(EntityID id) : id(id) {
      }

      EntityID getId() const {
        return id;
      }

      bool operator==(const Entity& other) const {
        return id == other.id;
      }

      bool operator!=(const Entity& other) const {
        return id != other.id;
      }

    private:
      EntityID id;
  };
}  // namespace ecs
