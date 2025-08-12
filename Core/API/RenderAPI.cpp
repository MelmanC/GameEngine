#include "RenderAPI.hpp"
#include "RenderComponent.hpp"

void api::RenderAPI::registerRenderTable(sol::state& lua) {
  lua["Render"] = lua.create_table();

  registerSetColor(lua);
  registerSetVisible(lua);
}

void api::RenderAPI::registerSetColor(sol::state& lua) {
  lua["Render"]["setColor"] = [this](Entity entity, int r, int g, int b,
                                     int a) {
    if (_ecsManager &&
        _ecsManager->hasComponent<ecs::RenderComponent>(entity)) {
      auto& render = _ecsManager->getComponent<ecs::RenderComponent>(entity);
      render.color = raylib::Color(r, g, b, a);
    }
  };
}

void api::RenderAPI::registerSetVisible(sol::state& lua) {
  lua["Render"]["setVisible"] = [this](Entity entity, bool visible) {
    if (_ecsManager &&
        _ecsManager->hasComponent<ecs::RenderComponent>(entity)) {
      auto& render = _ecsManager->getComponent<ecs::RenderComponent>(entity);
      render.visible = visible;
    }
  };
}
