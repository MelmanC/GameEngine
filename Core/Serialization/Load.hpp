#pragma once

#include <nlohmann/json.hpp>
#include <raylib-cpp.hpp>
#include "EntityManager.hpp"
#include "ShapeComponent.hpp"

namespace scene {
  class Scene;
}

namespace ecs {
  class ECSManager;
}

namespace jsonfile {

  class Load {
    public:
      Load() = default;
      ~Load() = default;

      void loadFromJson(scene::Scene& scene, const std::string& jsonFilePath,
                        ecs::ECSManager* ecsManager);

    private:
      void loadEntityFromJson(const nlohmann::json& entityData,
                              ecs::ECSManager* ecsManager);

      ecs::ShapeType getShapeTypeFromJson(const nlohmann::json& entityData);

      void loadTransformComponent(const nlohmann::json& entityData,
                                  Entity entity, ecs::ShapeType shapeType,
                                  ecs::ECSManager* ecsManager);

      void loadRenderComponent(const nlohmann::json& entityData, Entity entity,
                               ecs::ECSManager* ecsManager);

      void loadShapeComponent(Entity entity, ecs::ShapeType shapeType,
                              ecs::ECSManager* ecsManager);

      void loadNameComponent(const nlohmann::json& entityData, Entity entity,
                             ecs::ECSManager* ecsManager);

      void loadModelComponent(const nlohmann::json& entityData, Entity entity,
                              ecs::ECSManager* ecsManager);

      void loadDefaultComponents(Entity entity, ecs::ECSManager* ecsManager);

      void createCubeTransform(Entity entity, const raylib::Vector3& position,
                               const raylib::Vector3& rotation,
                               const nlohmann::json& transformData,
                               ecs::ECSManager* ecsManager);

      void createSphereTransform(Entity entity, const raylib::Vector3& position,
                                 const raylib::Vector3& rotation,
                                 const nlohmann::json& transformData,
                                 ecs::ECSManager* ecsManager);

      void createCylinderTransform(Entity entity,
                                   const raylib::Vector3& position,
                                   const raylib::Vector3& rotation,
                                   const nlohmann::json& transformData,
                                   ecs::ECSManager* ecsManager);

      void createPlaneTransform(Entity entity, const raylib::Vector3& position,
                                const raylib::Vector3& rotation,
                                const nlohmann::json& transformData,
                                ecs::ECSManager* ecsManager);

      void createModelTransform(Entity entity, const raylib::Vector3& position,
                                const raylib::Vector3& rotation,
                                const nlohmann::json& transformData,
                                ecs::ECSManager* ecsManager);

      void createDefaultTransformComponent(Entity entity,
                                           ecs::ShapeType shapeType,
                                           ecs::ECSManager* ecsManager);

      raylib::Vector3 getVector3FromJson(const nlohmann::json& data,
                                         const std::string& key,
                                         const raylib::Vector3& defaultValue = {
                                             0, 0, 0});

      raylib::Vector2 getVector2FromJson(const nlohmann::json& data,
                                         const std::string& key,
                                         const raylib::Vector2& defaultValue = {
                                             0, 0});
  };
}  // namespace jsonfile
