#pragma once

#include <memory>
#include <typeindex>
#include <unordered_map>
#include "EntityManager.hpp"
#include "System.hpp"

namespace ecs {

  class SystemManager {
    public:
      template <typename T>
      std::shared_ptr<T> registerSystem() {
        std::type_index ti(typeid(T));

        auto system = std::make_shared<T>();
        _systems.insert({ti, system});
        return system;
      }

      template <typename T>
      void setSignature(Signature signature) {
        std::type_index ti(typeid(T));
        _signatures.insert({ti, signature});
      }

      template <typename T>
      std::shared_ptr<T> getSystem() {
        std::type_index ti(typeid(T));
        auto it = _systems.find(ti);
        if (it != _systems.end()) {
          return std::static_pointer_cast<T>(it->second);
        }
        return nullptr;
      }

      void entityDestroyed(Entity entity) {
        for (auto &pair : _systems) {
          pair.second->_entities.erase(entity);
        }
      }

      void entitySignatureChanged(Entity entity, Signature signature) {
        for (auto &pair : _systems) {
          auto &system = pair.second;
          auto &systemSignature = _signatures[pair.first];

          if ((signature & systemSignature) == systemSignature) {
            system->_entities.insert(entity);
          } else {
            system->_entities.erase(entity);
          }
        }
      }

    private:
      std::unordered_map<std::type_index, Signature> _signatures;
      std::unordered_map<std::type_index, std::shared_ptr<System>> _systems;
  };

}  // namespace ecs
