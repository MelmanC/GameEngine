#pragma once

#include <nlohmann/json.hpp>
#include <raylib-cpp.hpp>
#include "EntityManager.hpp"
#include "ModelComponent.hpp"

namespace scene {
  class Scene;
}

namespace ecs {
  class ECSManager;
}

namespace jsonfile {

  class Save {
    public:
      Save() = default;
      ~Save() = default;

      void saveToJson(const scene::Scene& scene,
                      const std::string& jsonFilePath,
                      ecs::ECSManager* ecsManager) const;

    private:
      void saveEntityToJson(const Entity& entity, nlohmann::json& jsonData,
                            ecs::ECSManager* ecsManager) const;

      void saveTransformComponent(const Entity& entity,
                                  nlohmann::json& jsonData,
                                  ecs::ECSManager* ecsManager) const;

      void saveRenderComponent(const Entity& entity, nlohmann::json& jsonData,
                               ecs::ECSManager* ecsManager) const;

      void saveShapeComponent(const Entity& entity, nlohmann::json& jsonData,
                              ecs::ECSManager* ecsManager) const;

      void saveNameComponent(const Entity& entity, nlohmann::json& jsonData,
                             ecs::ECSManager* ecsManager) const;

      nlohmann::json saveModelComponent(const ecs::ModelComponent& model) const;

      void saveScriptComponent(const Entity& entity, nlohmann::json& jsonData,
                               ecs::ECSManager* ecsManager) const;
  };
}  // namespace jsonfile
