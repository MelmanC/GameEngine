#include "ScriptSystem.hpp"
#include <filesystem>
#include "Color.hpp"
#include "EntityManager.hpp"
#include "NameComponent.hpp"
#include "RenderComponent.hpp"
#include "ScriptComponent.hpp"
#include "TransformHelper.hpp"
#include "Vector3.hpp"

ecs::ScriptSystem::ScriptSystem() {
  _lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::table,
                      sol::lib::string, sol::lib::io, sol::lib::os);

  initalizeLuaBindings();
}

void ecs::ScriptSystem::update(float deltaTime) {
  for (Entity entity : _entities) {
    if (!_ecsManager->hasComponent<ScriptComponent>(entity))
      continue;

    auto& script = _ecsManager->getComponent<ScriptComponent>(entity);
    if (!script.enabled || script.state != ScriptState::LOADED)
      continue;

    if (!script.hasStarted) {
      executeStartFunction(entity);
      script.hasStarted = true;
    }

    executeUpdateFunction(entity, deltaTime);
  }
}

void ecs::ScriptSystem::loadScript(Entity entity,
                                   const std::string& scriptPath) {
  auto& script = _ecsManager->getComponent<ScriptComponent>(entity);

  if (script.luaRef != -1) {
    unloadScript(entity);
  }

  if (scriptPath.empty()) {
    script.state = ScriptState::NOT_LOADED;
    script.lastError.clear();
    script.hasStarted = false;
    return;
  }

  script.scriptPath = scriptPath;
  script.state = ScriptState::NOT_LOADED;
  script.lastError.clear();
  script.hasStarted = false;

  if (!std::filesystem::exists(scriptPath)) {
    script.state = ScriptState::ERROR;
    script.lastError = "Script file does not exist: " + scriptPath;
    std::cerr << script.lastError << std::endl;
    return;
  }

  int luaRef;
  if (loadScriptFile(scriptPath, luaRef)) {
    script.luaRef = luaRef;
    script.state = ScriptState::LOADED;
  } else {
    script.state = ScriptState::ERROR;
  }
}

void ecs::ScriptSystem::unloadScript(Entity entity) {
  auto& script = _ecsManager->getComponent<ScriptComponent>(entity);
  if (script.luaRef != -1) {
    executeDestroyFunction(entity);
    _lua[script.luaRef] = sol::nil;
    script.luaRef = -1;
  }
  script.state = ScriptState::NOT_LOADED;
  script.hasStarted = false;
}

void ecs::ScriptSystem::executeScript(Entity entity) {
  auto& script = _ecsManager->getComponent<ScriptComponent>(entity);

  if (script.state != ScriptState::LOADED || script.luaRef == -1)
    return;

  try {
    sol::function scriptFunc = _lua.registry()[script.luaRef];
    if (scriptFunc.valid()) {
      scriptFunc(entity);
    }
  } catch (const sol::error& e) {
    handleScriptError(entity, e.what());
  }
}

void ecs::ScriptSystem::executeScriptFunction(Entity entity,
                                              const std::string& functionName) {
  auto& script = _ecsManager->getComponent<ScriptComponent>(entity);

  if (script.state != ScriptState::LOADED || script.luaRef == -1)
    return;

  try {
    sol::table scriptTable = _lua.registry()[script.luaRef];
    if (scriptTable.valid()) {
      sol::function func = scriptTable[functionName];
      if (func.valid()) {
        func(entity);
      }
    }
  } catch (const sol::error& e) {
    handleScriptError(entity, e.what());
  }
}

void ecs::ScriptSystem::initalizeLuaBindings() {
  registerEntityAPI();
  registerComponentAPI();
}

void ecs::ScriptSystem::registerEntityAPI() {
  _lua["Entity"] = _lua.create_table();

  _lua["Entity"]["getName"] = [this](Entity entity) -> std::string {
    if (_ecsManager && _ecsManager->hasComponent<NameComponent>(entity)) {
      return _ecsManager->getComponent<NameComponent>(entity).name;
    }
    return "";
  };

  _lua["Entity"]["setName"] = [this](Entity entity, const std::string& name) {
    if (_ecsManager && _ecsManager->hasComponent<NameComponent>(entity)) {
      _ecsManager->getComponent<NameComponent>(entity).name = name;
    }
  };
}

void ecs::ScriptSystem::registerComponentAPI() {
  _lua["Transform"] = _lua.create_table();

  _lua["Transform"]["getPosition"] = [this](Entity entity) -> sol::table {
    if (!_ecsManager)
      return sol::nil;

    raylib::Vector3 pos =
        ecs::TransformHelper::getPosition(entity, _ecsManager);
    sol::table result = _lua.create_table();
    result["x"] = pos.x;
    result["y"] = pos.y;
    result["z"] = pos.z;
    return result;
  };

  _lua["Transform"]["setPosition"] = [this](Entity entity, float x, float y,
                                            float z) {
    if (_ecsManager) {
      TransformHelper::setPosition(entity, {x, y, z}, _ecsManager);
    }
  };

  _lua["Transform"]["move"] = [this](Entity entity, float x, float y, float z) {
    if (_ecsManager) {
      TransformHelper::movePosition(entity, {x, y, z}, _ecsManager);
    }
  };

  _lua["Render"] = _lua.create_table();

  _lua["Render"]["setColor"] = [this](Entity entity, int r, int g, int b,
                                      int a) {
    if (_ecsManager && _ecsManager->hasComponent<RenderComponent>(entity)) {
      auto& render = _ecsManager->getComponent<RenderComponent>(entity);
      render.color = raylib::Color(r, g, b, a);
    }
  };

  _lua["Render"]["setVisible"] = [this](Entity entity, bool visible) {
    if (_ecsManager && _ecsManager->hasComponent<RenderComponent>(entity)) {
      auto& render = _ecsManager->getComponent<RenderComponent>(entity);
      render.visible = visible;
    }
  };
}

bool ecs::ScriptSystem::loadScriptFile(const std::string& scriptPath,
                                       int& luaRef) {
  try {
    sol::load_result script = _lua.load_file(scriptPath);
    if (!script.valid()) {
      sol::error err = script;
      std::cerr << "Failed to load script file: " << scriptPath
                << "\nError: " << err.what() << std::endl;
      return false;
    }

    sol::protected_function_result result = script();
    if (!result.valid()) {
      sol::error err = result;
      std::cerr << "Failed to execute script file: " << scriptPath
                << "\nError: " << err.what() << std::endl;
      return false;
    }

    luaRef = _lua.registry().size() + 1;
    _lua.registry()[luaRef] = result;

    return true;
  } catch (const sol::error& e) {
    std::cerr << "Script error: " << e.what() << std::endl;
    return false;
  }
}

void ecs::ScriptSystem::executeStartFunction(Entity entity) {
  executeScriptFunction(entity, "OnStart");
}

void ecs::ScriptSystem::executeUpdateFunction(Entity entity, float deltaTime) {
  if (!_ecsManager) {
    return;
  }

  auto& script = _ecsManager->getComponent<ScriptComponent>(entity);

  if (script.state != ScriptState::LOADED || script.luaRef == -1) {
    return;
  }

  try {
    sol::table scriptTable = _lua.registry()[script.luaRef];
    if (scriptTable.valid()) {
      sol::function updateFunc = scriptTable["OnUpdate"];
      if (updateFunc.valid()) {
        updateFunc(entity, deltaTime);
      }
    }
  } catch (const sol::error& e) {
    handleScriptError(entity, e.what());
  }
}

void ecs::ScriptSystem::executeDestroyFunction(Entity entity) {
  executeScriptFunction(entity, "OnDestroy");
}

void ecs::ScriptSystem::handleScriptError(Entity entity,
                                          const std::string& error) {
  if (!_ecsManager) {
    return;
  }

  auto& script = _ecsManager->getComponent<ScriptComponent>(entity);
  script.state = ScriptState::ERROR;
  script.lastError = error;

  std::cerr << "Script error for entity " << entity << ": " << error
            << std::endl;
}
