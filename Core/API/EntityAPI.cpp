#include "EntityAPI.hpp"
#include "NameComponent.hpp"

void api::EntityAPI::registerEntityTable(sol::state& lua) {
  lua["Entity"] = lua.create_table();

  registerGetName(lua);
  registerSetName(lua);
}

void api::EntityAPI::registerGetName(sol::state& lua) {
  lua["Entity"]["getName"] = [this](Entity entity) -> std::string {
    if (_ecsManager && _ecsManager->hasComponent<ecs::NameComponent>(entity)) {
      return _ecsManager->getComponent<ecs::NameComponent>(entity).name;
    }
    return "";
  };
}

void api::EntityAPI::registerSetName(sol::state& lua) {
  lua["Entity"]["setName"] = [this](Entity entity, const std::string& name) {
    if (_ecsManager && _ecsManager->hasComponent<ecs::NameComponent>(entity)) {
      _ecsManager->getComponent<ecs::NameComponent>(entity).name = name;
    }
  };
}
