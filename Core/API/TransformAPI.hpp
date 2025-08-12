#pragma once

#include <sol/sol.hpp>
#include "ECSManager.hpp"

namespace api {

  class TransformAPI {
    public:
      TransformAPI(ecs::ECSManager* ecsManager) : _ecsManager(ecsManager) {
      }
      ~TransformAPI() = default;

      void registerTransformTable(sol::state& lua);

      void registerGetPosition(sol::state& lua);
      void registerSetPosition(sol::state& lua);

      void registerMove(sol::state& lua);

      void setECSManager(ecs::ECSManager* ecsManager) {
        _ecsManager = ecsManager;
      }

    private:
      ecs::ECSManager* _ecsManager = nullptr;
  };
}  // namespace api
