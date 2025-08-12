#pragma once

#include <sol/sol.hpp>
#include <string>
#include "ECSManager.hpp"
#include "System.hpp"

namespace ecs {

  class ScriptSystem : public System {
    public:
      ScriptSystem();
      ~ScriptSystem() = default;

      void setECSManager(ECSManager* ecsManager) {
        _ecsManager = ecsManager;
      }

      void update(float deltaTime) override;

      void loadScript(Entity entity, const std::string& scriptPath);
      void unloadScript(Entity entity);

      void executeScript(Entity entity);
      void executeScriptFunction(Entity entity,
                                 const std::string& functionName);

      sol::state& getLuaState() {
        return _lua;
      }

      void initalizeLuaBindings();
      void registerEntityAPI();
      void registerComponentAPI();

      bool loadScriptFile(const std::string& scriptPath, int& luaRef);
      void executeStartFunction(Entity entity);
      void executeUpdateFunction(Entity entity, float deltaTime);
      void executeDestroyFunction(Entity entity);

      void handleScriptError(Entity entity, const std::string& error);

    private:
      sol::state _lua;
      ECSManager* _ecsManager = nullptr;
  };
}  // namespace ecs
