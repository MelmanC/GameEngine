#pragma once

#include "ComponentManager.hpp"
#include "EntityManager.hpp"
#include "SystemManager.hpp"

namespace ecs {

  class ECSManager {
    public:
      ECSManager()
          : _entityManager(std::make_unique<EntityManager>()),
            _componentManager(std::make_unique<ComponentManager>()),
            _systemManager(std::make_unique<SystemManager>()) {
      }

      Entity createEntity() {
        return _entityManager->createEntity();
      }

      void destroyEntity(Entity entity) {
        _entityManager->destroyEntity(entity);
        _componentManager->entityDestroyed(entity);
        _systemManager->entityDestroyed(entity);
      }

      std::vector<Entity> getAllEntities() const {
        return _entityManager->getAllLivingEntities();
      }

      int getEntityCount() const {
        return _entityManager->getEntityCount();
      }

      template <typename T>
      bool hasComponent(Entity entity) {
        return _componentManager->hasComponent<T>(entity);
      }

      template <typename T>
      void registerComponent() {
        _componentManager->registerComponent<T>();
      }

      template <typename T>
      void addComponent(Entity entity, T component) {
        _componentManager->addComponent<T>(entity, component);

        auto signature = _entityManager->getSignature(entity);
        signature.set(_componentManager->getComponentType<T>(), true);
        _entityManager->setSignature(entity, signature);
        _systemManager->entitySignatureChanged(entity, signature);
      }

      template <typename T>
      void removeComponent(Entity entity) {
        _componentManager->removeComponent<T>(entity);

        auto signature = _entityManager->getSignature(entity);
        signature.set(_componentManager->getComponentType<T>(), false);
        _entityManager->setSignature(entity, signature);
        _systemManager->entitySignatureChanged(entity, signature);
      }

      template <typename T>
      T &getComponent(Entity entity) {
        return _componentManager->getComponent<T>(entity);
      }

      template <typename T>
      std::shared_ptr<T> getSystem() {
        return _systemManager->getSystem<T>();
      }

      template <typename T>
      ComponentType getComponentType() {
        return _componentManager->getComponentType<T>();
      }

      template <typename T>
      std::shared_ptr<T> registerSystem() {
        return _systemManager->registerSystem<T>();
      }

      template <typename T>
      void setSystemeSignature(Signature signature) {
        _systemManager->setSignature<T>(signature);
      }

    private:
      std::unique_ptr<EntityManager> _entityManager;
      std::unique_ptr<ComponentManager> _componentManager;
      std::unique_ptr<SystemManager> _systemManager;
  };

}  // namespace ecs
