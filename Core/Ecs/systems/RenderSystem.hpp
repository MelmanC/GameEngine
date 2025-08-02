#pragma once

#include "ComponentManager.hpp"
#include "System.hpp"

namespace ecs {

  class RenderSystem : public System {
    public:
      RenderSystem(ComponentManager &manager) : _componentManager(manager) {
      }

      void update(float deltaTime);

      void render();

    private:
      ComponentManager &_componentManager;
  };

}  // namespace ecs
//
