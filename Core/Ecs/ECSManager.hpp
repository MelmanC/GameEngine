#pragma once

#include "ComponentManager.hpp"
#include "EntityManager.hpp"
#include "System.hpp"

namespace ecs {

  class ECSManager {
    public:
      ECSManager() = default;

      ~ECSManager() = default;

      Entity createEntity() {
        return _entityManager.createEntity();
      }

      void destroyEntity(Entity entity) {
        _entityManager.destroyEntity(entity);
      }

      const std::vector<Entity> &getEntities() const {
        return _entityManager.getEntities();
      }

      template <typename T, typename... Args>
      T &addComponent(Entity entity, Args &&...args) {
        return _componentManager.addComponent<T>(entity,
                                                 std::forward<Args>(args)...);
      }

      template <typename T>
      T *getComponent(Entity entity) {
        return _componentManager.getComponent<T>(entity);
      }

      template <typename T>
      bool hasComponent(Entity entity) {
        return _componentManager.hasComponent<T>(entity);
      }

      template <typename T>
      void removeComponent(Entity entity) {
        _componentManager.removeComponent<T>(entity);
      }

      template <typename T, typename... Args>
      T *addSystem(Args &&...args) {
        auto system = std::make_unique<T>(this, std::forward<Args>(args)...);
        T *systemPtr = system.get();
        _systems.push_back(std::move(system));
        return systemPtr;
      }

      void updateSystems() {
        for (const auto &system : _systems) {
          system->update();
        }
      }

    private:
      EntityManager _entityManager;
      ComponentManager _componentManager;
      std::vector<std::unique_ptr<System>> _systems;
  };

}  // namespace ecs
