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
    nlohmann::json entityData;
    entityData["id"] = entity;

    auto& shape = ecsManager->getComponent<ecs::ShapeComponent>(entity);

    nlohmann::json transformData;
    switch (shape.type) {
      case ecs::ShapeType::CUBE: {
        auto& transform =
            ecsManager->getComponent<ecs::CubeTransformComponent>(entity);
        transformData = {{"position",
                          {{"x", transform.position.x},
                           {"y", transform.position.y},
                           {"z", transform.position.z}}},
                         {"rotation",
                          {{"x", transform.rotation.x},
                           {"y", transform.rotation.y},
                           {"z", transform.rotation.z}}},
                         {"size",
                          {{"x", transform.size.x},
                           {"y", transform.size.y},
                           {"z", transform.size.z}}}};
        break;
      }
      case ecs::ShapeType::SPHERE: {
        auto& transform =
            ecsManager->getComponent<ecs::SphereTransformComponent>(entity);
        transformData = {{"position",
                          {{"x", transform.position.x},
                           {"y", transform.position.y},
                           {"z", transform.position.z}}},
                         {"rotation",
                          {{"x", transform.rotation.x},
                           {"y", transform.rotation.y},
                           {"z", transform.rotation.z}}},
                         {"radius", transform.radius}};
        break;
      }
      case ecs::ShapeType::CYLINDER: {
        auto& transform =
            ecsManager->getComponent<ecs::CylinderTransformComponent>(entity);
        transformData = {{"position",
                          {{"x", transform.position.x},
                           {"y", transform.position.y},
                           {"z", transform.position.z}}},
                         {"rotation",
                          {{"x", transform.rotation.x},
                           {"y", transform.rotation.y},
                           {"z", transform.rotation.z}}},
                         {"radius", transform.radius},
                         {"height", transform.height}};
        break;
      }
      case ecs::ShapeType::PLANE: {
        auto& transform =
            ecsManager->getComponent<ecs::PlaneTransformComponent>(entity);
        transformData = {
            {"position",
             {{"x", transform.position.x},
              {"y", transform.position.y},
              {"z", transform.position.z}}},
            {"rotation",
             {{"x", transform.rotation.x},
              {"y", transform.rotation.y},
              {"z", transform.rotation.z}}},
            {"size", {{"x", transform.size.x}, {"y", transform.size.y}}}};
        break;
      }
    }
    entityData["transform"] = transformData;

    auto& render = ecsManager->getComponent<ecs::RenderComponent>(entity);
    entityData["render"] = {{"color",
                             {{"r", render.color.r},
                              {"g", render.color.g},
                              {"b", render.color.b},
                              {"a", render.color.a}}},
                            {"visible", render.visible}};

    entityData["shape"] = {{"type", shape.getShapeTypeString()}};

    auto& name = ecsManager->getComponent<ecs::NameComponent>(entity);
    entityData["name"] = {{"name", name.name}, {"type", name.type}};

    jsonData["entities"].push_back(entityData);
  }

  std::ofstream file(jsonFilePath);
  if (!file.is_open()) {
    throw std::runtime_error("Could not open JSON file for writing: " +
                             jsonFilePath);
  }
  file << jsonData.dump(2);
}
