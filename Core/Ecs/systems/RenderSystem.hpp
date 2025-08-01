#pragma once

#include "ECSManager.hpp"
#include "Entity.hpp"
#include "System.hpp"

namespace ecs {

  class RenderSystem : public System {
    public:
      RenderSystem(ECSManager *manager) : System(manager) {
      }

      void update() override;

      void render();

      void renderCube(Entity entity);

      void renderSelectedOutline(Entity entity);

      bool hasRequiredComponents(Entity entity) const;
  };
}  // namespace ecs
