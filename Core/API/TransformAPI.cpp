#include "TransformAPI.hpp"
#include "TransformHelper.hpp"

void api::TransformAPI::registerTransformTable(sol::state& lua) {
  lua["Transform"] = lua.create_table();

  registerGetPosition(lua);
  registerSetPosition(lua);

  registerMove(lua);
}

void api::TransformAPI::registerGetPosition(sol::state& lua) {
  lua["Transform"]["getPosition"] = [&, this](Entity entity) -> sol::table {
    if (!_ecsManager)
      return sol::nil;

    raylib::Vector3 pos =
        ecs::TransformHelper::getPosition(entity, _ecsManager);
    sol::table result = lua.create_table();
    result["x"] = pos.x;
    result["y"] = pos.y;
    result["z"] = pos.z;
    return result;
  };
}

void api::TransformAPI::registerSetPosition(sol::state& lua) {
  lua["Transform"]["setPosition"] = [this](Entity entity, float x, float y,
                                           float z) {
    if (_ecsManager) {
      ecs::TransformHelper::setPosition(entity, {x, y, z}, _ecsManager);
    }
  };
}

void api::TransformAPI::registerMove(sol::state& lua) {
  lua["Transform"]["move"] = [this](Entity entity, float x, float y, float z) {
    if (_ecsManager) {
      ecs::TransformHelper::movePosition(entity, {x, y, z}, _ecsManager);
    }
  };
}
