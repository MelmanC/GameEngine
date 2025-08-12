#pragma once

#include <sol/state.hpp>
#include "ECSManager.hpp"

namespace api {

  class InputAPI {
    public:
      InputAPI(ecs::ECSManager* ecsManager) : _ecsManager(ecsManager) {
      }

      void registerInputTable(sol::state& lua);

      void registerIsKeyPressed(sol::state& lua);
      void registerIsKeyDown(sol::state& lua);

    private:
      ecs::ECSManager* _ecsManager = nullptr;
  };

}  // namespace api
