#pragma once

#include <sol/state.hpp>
#include "ECSManager.hpp"

namespace api {

  class RenderAPI {
    public:
      RenderAPI(ecs::ECSManager* ecsManager) : _ecsManager(ecsManager) {
      }

      void registerRenderTable(sol::state& lua);

      void registerSetColor(sol::state& lua);
      void registerSetVisible(sol::state& lua);

    private:
      ecs::ECSManager* _ecsManager = nullptr;
  };

}  // namespace api
