#pragma once

#include <typeindex>
#include <unordered_map>
#include "Component.hpp"
#include "Entity.hpp"

namespace ecs {
  class ComponentManager {
    public:
      ComponentManager() = default;
      ~ComponentManager() = default;

      template <typename T, typename... Args>
      T *addComponent(Entity entity, Args &&...args) {
        auto &componentArray = getComponentArray<T>();
        componentArray[entity.getId()] =
            std::make_unique<T>(std::forward<Args>(args)...);

        return static_cast<T *>(componentArray[entity.getId()].get());
      }

      template <typename T>
      T *getComponent(Entity entity) {
        auto &componentArray = getComponentArray<T>();
        auto it = componentArray.find(entity.getId());
        if (it != componentArray.end()) {
          return static_cast<T *>(it->second.get());
        }
        return nullptr;
      }

      template <typename T>
      bool hasComponent(Entity entity) {
        auto &componentArray = getComponentArray<T>();
        return componentArray.find(entity.getId()) != componentArray.end();
      }

      template <typename T>
      void removeComponent(Entity entity) {
        auto &componentArray = getComponentArray<T>();
        componentArray.erase(entity.getId());
      }

      template <typename T>
      void removeAllComponents(Entity entity) {
        for (auto &pair : _components) {
          auto &componentArray = pair.second;
          componentArray.erase(entity.getId());
        }
      }

    private:
      std::unordered_map<
          std::type_index,
          std::unordered_map<EntityID, std::unique_ptr<BaseComponent>>>
          _components;

      template <typename T>
      std::unordered_map<EntityID, std::unique_ptr<BaseComponent>> &
      getComponentArray() {
        std::type_index typeIndex = std::type_index(typeid(T));
        return _components[typeIndex];
      }
  };
}  // namespace ecs
