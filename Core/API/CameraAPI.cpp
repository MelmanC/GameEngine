#include "CameraAPI.hpp"
#include "CameraComponent.hpp"

void api::CameraAPI::registerCameraTable(sol::state &lua) {
  lua["Camera"] = lua.create_table();

  registerGetCameraPosition(lua);
  registerSetCameraPosition(lua);
}

void api::CameraAPI::registerGetCameraPosition(sol::state &lua) {
  lua["Camera"]["getPosition"] = [&, this](Entity entity) -> sol::table {
    if (!_ecsManager)
      return sol::nil;

    raylib::Vector3 pos =
        _ecsManager->getComponent<ecs::CameraComponent>(entity).position;
    sol::table result = lua.create_table();
    result["x"] = pos.x;
    result["y"] = pos.y;
    result["z"] = pos.z;
    return result;
  };
}

void api::CameraAPI::registerSetCameraPosition(sol::state &lua) {
  lua["Camera"]["setPosition"] = [this](Entity entity, float x, float y,
                                        float z) {
    if (_ecsManager) {
      auto &cameraComp =
          _ecsManager->getComponent<ecs::CameraComponent>(entity);
      cameraComp.position = raylib::Vector3(x, y, z);
    }
  };
}
