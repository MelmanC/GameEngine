#include "InputAPI.hpp"
#include <raylib.h>

void api::InputAPI::registerInputTable(sol::state& lua) {
  lua["Input"] = lua.create_table();

  registerIsKeyPressed(lua);
  registerIsKeyDown(lua);
}

void api::InputAPI::registerIsKeyPressed(sol::state& lua) {
  lua["Input"]["isKeyPressed"] = [](int key) -> bool {
    return IsKeyPressed(static_cast<KeyboardKey>(key));
  };
}

void api::InputAPI::registerIsKeyDown(sol::state& lua) {
  lua["Input"]["isKeyDown"] = [](int key) -> bool {
    return IsKeyDown(static_cast<KeyboardKey>(key));
  };
}
