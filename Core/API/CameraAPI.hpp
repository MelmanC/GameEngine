#pragma once

#include <sol/state.hpp>
#include "ECSManager.hpp"

namespace api {

  class CameraAPI {
    public:
      CameraAPI(ecs::ECSManager* ecsManager) : _ecsManager(ecsManager) {
      }

      void registerCameraTable(sol::state& lua);

      void registerGetCameraPosition(sol::state& lua);

      void registerSetCameraPosition(sol::state& lua);

    private:
      ecs::ECSManager* _ecsManager = nullptr;
  };

}  // namespace api
