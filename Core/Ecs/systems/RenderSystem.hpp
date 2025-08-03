#pragma once

#include "System.hpp"

namespace ecs {
  class ECSManager;
}  // namespace ecs

namespace ecs {

  class RenderSystem : public System {
    public:
      RenderSystem() = default;

      void setECSManager(ECSManager *ecsManager) {
        _ecsManager = ecsManager;
      }

      void update(float deltaTime);

      void render();

    private:
      ECSManager *_ecsManager = nullptr;
  };

}  // namespace ecs
//
