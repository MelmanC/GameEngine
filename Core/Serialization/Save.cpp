#include "Save.hpp"
#include <fstream>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>
#include <vector>
#include "ECSManager.hpp"
#include "NameComponent.hpp"
#include "RenderComponent.hpp"
#include "Scene.hpp"
#include "ShapeComponent.hpp"
#include "TransformComponent.hpp"
#include "TransformHelper.hpp"

void jsonfile::Save::saveToJson(const scene::Scene& scene,
                                const std::string& jsonFilePath,
                                ecs::ECSManager* ecsManager) const {
  if (!ecsManager) {
    throw std::runtime_error("ECSManager is null");
  }

  nlohmann::json jsonData;
  jsonData["entities"] = nlohmann::json::array();

  std::vector<Entity> entities = scene.getAllEntities();

  for (Entity entity : entities) {
    saveEntityToJson(entity, jsonData, ecsManager);
  }

  std::ofstream file(jsonFilePath);
  if (!file.is_open()) {
    throw std::runtime_error("Could not open JSON file for writing: " +
                             jsonFilePath);
  }
  file << jsonData.dump(2);
}

void jsonfile::Save::saveEntityToJson(const Entity& entity,
                                      nlohmann::json& jsonData,
                                      ecs::ECSManager* ecsManager) const {
  if (!ecsManager) {
    throw std::runtime_error("ECSManager is null");
  }

  nlohmann::json entityData;
  entityData["id"] = entity;
  saveTransformComponent(entity, entityData, ecsManager);
  saveRenderComponent(entity, entityData, ecsManager);
  saveShapeComponent(entity, entityData, ecsManager);
  saveNameComponent(entity, entityData, ecsManager);

  jsonData["entities"].push_back(entityData);
}

void jsonfile::Save::saveTransformComponent(const Entity& entity,
                                            nlohmann::json& jsonData,
                                            ecs::ECSManager* ecsManager) const {
  if (!ecsManager) {
    throw std::runtime_error("ECSManager is null");
  }

  auto& shape = ecsManager->getComponent<ecs::ShapeComponent>(entity);

  raylib::Vector3 position =
      ecs::TransformHelper::getPosition(entity, ecsManager);
  raylib::Vector3 rotation =
      ecs::TransformHelper::getRotation(entity, ecsManager);

  nlohmann::json transformData;

  transformData["position"] = {
      {"x", position.x}, {"y", position.y}, {"z", position.z}};

  transformData["rotation"] = {
      {"x", rotation.x}, {"y", rotation.y}, {"z", rotation.z}};

  switch (shape.type) {
    case ecs::ShapeType::CUBE: {
      auto& transform =
          ecsManager->getComponent<ecs::CubeTransformComponent>(entity);
      transformData["size"] = {{"x", transform.size.x},
                               {"y", transform.size.y},
                               {"z", transform.size.z}};
      break;
    }
    case ecs::ShapeType::SPHERE: {
      auto& transform =
          ecsManager->getComponent<ecs::SphereTransformComponent>(entity);
      transformData["radius"] = transform.radius;
      break;
    }
    case ecs::ShapeType::CYLINDER: {
      auto& transform =
          ecsManager->getComponent<ecs::CylinderTransformComponent>(entity);
      transformData["radius"] = transform.radius;
      transformData["height"] = transform.height;
      break;
    }
    case ecs::ShapeType::PLANE: {
      auto& transform =
          ecsManager->getComponent<ecs::PlaneTransformComponent>(entity);
      transformData["size"] = {{"x", transform.size.x},
                               {"y", transform.size.y}};
      break;
    }
    default:
      throw std::runtime_error("Unknown shape type for entity " +
                               std::to_string(entity));
  }

  jsonData["transform"] = transformData;
}

void jsonfile::Save::saveRenderComponent(const Entity& entity,
                                         nlohmann::json& jsonData,
                                         ecs::ECSManager* ecsManager) const {
  if (!ecsManager) {
    throw std::runtime_error("ECSManager is null");
  }

  auto& render = ecsManager->getComponent<ecs::RenderComponent>(entity);
  jsonData["render"] = {{"color",
                         {{"r", render.color.r},
                          {"g", render.color.g},
                          {"b", render.color.b},
                          {"a", render.color.a}}},
                        {"visible", render.visible}};
}

void jsonfile::Save::saveShapeComponent(const Entity& entity,
                                        nlohmann::json& jsonData,
                                        ecs::ECSManager* ecsManager) const {
  if (!ecsManager) {
    throw std::runtime_error("ECSManager is null");
  }

  auto& shape = ecsManager->getComponent<ecs::ShapeComponent>(entity);
  jsonData["shape"] = {{"type", shape.getShapeTypeString()}};
}

void jsonfile::Save::saveNameComponent(const Entity& entity,
                                       nlohmann::json& jsonData,
                                       ecs::ECSManager* ecsManager) const {
  if (!ecsManager) {
    throw std::runtime_error("ECSManager is null");
  }

  auto& name = ecsManager->getComponent<ecs::NameComponent>(entity);
  jsonData["name"] = {{"name", name.name}, {"type", name.type}};
}
