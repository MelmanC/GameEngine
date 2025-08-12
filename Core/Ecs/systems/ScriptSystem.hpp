#pragma once

#include <sol/sol.hpp>
#include <string>
#include "ECSManager.hpp"
#include "EntityAPI.hpp"
#include "InputAPI.hpp"
#include "RenderAPI.hpp"
#include "System.hpp"
#include "TransformAPI.hpp"

namespace ecs {

  class ScriptSystem : public System {
    public:
      ScriptSystem();
      ~ScriptSystem() = default;

      void setECSManager(ECSManager* ecsManager) {
        _ecsManager = ecsManager;
        initalizeLuaBindings();
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
      std::unique_ptr<api::EntityAPI> _entityAPI;
      std::unique_ptr<api::TransformAPI> _transformAPI;
      std::unique_ptr<api::RenderAPI> _renderAPI;
      std::unique_ptr<api::InputAPI> _inputAPI;
  };
}  // namespace ecs
