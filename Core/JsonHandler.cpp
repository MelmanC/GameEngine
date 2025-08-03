#include "JsonHandler.hpp"
#include <raylib.h>
#include <fstream>
#include <nlohmann/json.hpp>
#include <raylib-cpp.hpp>
#include "ECSManager.hpp"
#include "NameComponent.hpp"
#include "RenderComponent.hpp"
#include "Scene.hpp"
#include "SelectionComponent.hpp"
#include "ShapeComponent.hpp"
#include "TransformComponent.hpp"

void jsonfile::JsonHandler::loadFromJson(scene::Scene& scene,
                                         const std::string& jsonFilePath,
                                         ecs::ECSManager* ecsManager) {
  if (!ecsManager) {
    throw std::runtime_error("ECSManager is null");
  }

  std::ifstream file(jsonFilePath);
  if (!file.is_open()) {
    throw std::runtime_error("Could not open JSON file: " + jsonFilePath);
  }

  nlohmann::json jsonData;
  file >> jsonData;

  scene.clearEntities();

  for (const auto& entityData : jsonData["entities"]) {
    Entity entity = ecsManager->createEntity();

    if (entityData.contains("transform")) {
      const auto& transformData = entityData["transform"];
      ecs::TransformComponent transform;
      transform.position = raylib::Vector3(transformData["position"]["x"],
                                           transformData["position"]["y"],
                                           transformData["position"]["z"]);
      transform.size = raylib::Vector3(transformData["size"]["x"],
                                       transformData["size"]["y"],
                                       transformData["size"]["z"]);
      ecsManager->addComponent(entity, transform);
    }

    if (entityData.contains("render")) {
      const auto& renderData = entityData["render"];
      ecs::RenderComponent render;
      render.color =
          raylib::Color(renderData["color"]["r"], renderData["color"]["g"],
                        renderData["color"]["b"], renderData["color"]["a"]);
      render.visible = renderData.value("visible", true);
      ecsManager->addComponent(entity, render);
    }

    if (entityData.contains("shape")) {
      const auto& shapeData = entityData["shape"];
      ecs::ShapeComponent shape;
      std::string shapeType = shapeData.value("type", "CUBE");
      if (shapeType == "CUBE") {
        shape.type = ecs::ShapeType::CUBE;
      } else if (shapeType == "SPHERE") {
        shape.type = ecs::ShapeType::SPHERE;
      } else if (shapeType == "CYLINDER") {
        shape.type = ecs::ShapeType::CYLINDER;
      } else if (shapeType == "PLANE") {
        shape.type = ecs::ShapeType::PLANE;
      }
      ecsManager->addComponent(entity, shape);
    }

    if (entityData.contains("name")) {
      const auto& nameData = entityData["name"];
      ecs::NameComponent name;
      name.name = nameData.value("name", "Entity");
      name.type = nameData.value("type", "Unknown");
      ecsManager->addComponent(entity, name);
    }

    ecs::SelectionComponent selection;
    selection.selected = false;
    ecsManager->addComponent(entity, selection);
  }
}

void jsonfile::JsonHandler::saveToJson(const scene::Scene& scene,
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

    try {
      // Sauvegarder TransformComponent
      auto& transform =
          ecsManager->getComponent<ecs::TransformComponent>(entity);
      entityData["transform"] = {{"position",
                                  {{"x", transform.position.x},
                                   {"y", transform.position.y},
                                   {"z", transform.position.z}}},
                                 {"size",
                                  {{"x", transform.size.x},
                                   {"y", transform.size.y},
                                   {"z", transform.size.z}}}};
    } catch (const std::exception&) {
    }

    try {
      auto& render = ecsManager->getComponent<ecs::RenderComponent>(entity);
      entityData["render"] = {{"color",
                               {{"r", render.color.r},
                                {"g", render.color.g},
                                {"b", render.color.b},
                                {"a", render.color.a}}},
                              {"visible", render.visible}};
    } catch (const std::exception&) {
    }

    try {
      auto& shape = ecsManager->getComponent<ecs::ShapeComponent>(entity);
      entityData["shape"] = {{"type", shape.getShapeTypeString()}};
    } catch (const std::exception&) {
    }

    try {
      auto& name = ecsManager->getComponent<ecs::NameComponent>(entity);
      entityData["name"] = {{"name", name.name}, {"type", name.type}};
    } catch (const std::exception&) {
    }

    jsonData["entities"].push_back(entityData);
  }

  std::ofstream file(jsonFilePath);
  if (!file.is_open()) {
    throw std::runtime_error("Could not open JSON file for writing: " +
                             jsonFilePath);
  }
  file << jsonData.dump(2);
}
