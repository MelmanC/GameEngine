#pragma once

#include <sol/state.hpp>
#include "ECSManager.hpp"

namespace api {

  class EntityAPI {
    public:
      EntityAPI(ecs::ECSManager* ecsManager) : _ecsManager(ecsManager) {
      }

      void registerEntityTable(sol::state& lua);

      void registerGetName(sol::state& lua);
      void registerSetName(sol::state& lua);

    private:
      ecs::ECSManager* _ecsManager = nullptr;
  };

}  // namespace api
