#include "Load.hpp"
#include <raylib.h>
#include <fstream>
#include <nlohmann/json.hpp>
#include <raylib-cpp.hpp>
#include "ECSManager.hpp"
#include "GizmoComponent.hpp"
#include "GizmoSystem.hpp"
#include "NameComponent.hpp"
#include "RenderComponent.hpp"
#include "Scene.hpp"
#include "SelectionComponent.hpp"
#include "ShapeComponent.hpp"
#include "TransformComponent.hpp"

void jsonfile::Load::loadFromJson(scene::Scene& scene,
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
    loadEntityFromJson(entityData, ecsManager);
  }
}

void jsonfile::Load::loadEntityFromJson(const nlohmann::json& entityData,
                                        ecs::ECSManager* ecsManager) {
  Entity entity = ecsManager->createEntity();

  ecs::ShapeType shapeType = getShapeTypeFromJson(entityData);

  loadTransformComponent(entityData, entity, shapeType, ecsManager);
  loadRenderComponent(entityData, entity, ecsManager);
  loadShapeComponent(entity, shapeType, ecsManager);
  loadNameComponent(entityData, entity, ecsManager);
  loadDefaultComponents(entity, ecsManager);
}

ecs::ShapeType jsonfile::Load::getShapeTypeFromJson(
    const nlohmann::json& entityData) {
  if (!entityData.contains("shape")) {
    return ecs::ShapeType::CUBE;
  }

  const auto& shapeData = entityData["shape"];
  std::string shapeTypeStr = shapeData.value("type", "CUBE");

  if (shapeTypeStr == "Cube")
    return ecs::ShapeType::CUBE;
  if (shapeTypeStr == "Sphere")
    return ecs::ShapeType::SPHERE;
  if (shapeTypeStr == "Cylinder")
    return ecs::ShapeType::CYLINDER;
  if (shapeTypeStr == "Plane")
    return ecs::ShapeType::PLANE;

  return ecs::ShapeType::CUBE;
}

void jsonfile::Load::loadTransformComponent(const nlohmann::json& entityData,
                                            Entity entity,
                                            ecs::ShapeType shapeType,
                                            ecs::ECSManager* ecsManager) {
  if (!entityData.contains("transform"))
    throw std::runtime_error(
        "Entity data does not contain 'transform' component");

  const auto& transformData = entityData["transform"];
  raylib::Vector3 position = getVector3FromJson(transformData, "position");
  raylib::Vector3 rotation =
      getVector3FromJson(transformData, "rotation", {0, 0, 0});

  switch (shapeType) {
    case ecs::ShapeType::CUBE:
      createCubeTransform(entity, position, rotation, transformData,
                          ecsManager);
      break;
    case ecs::ShapeType::SPHERE:
      createSphereTransform(entity, position, rotation, transformData,
                            ecsManager);
      break;
    case ecs::ShapeType::CYLINDER:
      createCylinderTransform(entity, position, rotation, transformData,
                              ecsManager);
      break;
    case ecs::ShapeType::PLANE:
      createPlaneTransform(entity, position, rotation, transformData,
                           ecsManager);
      break;
  }
}

void jsonfile::Load::createCubeTransform(Entity entity,
                                         const raylib::Vector3& position,
                                         const raylib::Vector3& rotation,
                                         const nlohmann::json& transformData,
                                         ecs::ECSManager* ecsManager) {
  ecs::CubeTransformComponent transform;
  transform.position = position;
  transform.rotation = rotation;
  transform.size = getVector3FromJson(transformData, "size", {1, 1, 1});
  ecsManager->addComponent(entity, transform);
}

void jsonfile::Load::createSphereTransform(Entity entity,
                                           const raylib::Vector3& position,
                                           const raylib::Vector3& rotation,
                                           const nlohmann::json& transformData,
                                           ecs::ECSManager* ecsManager) {
  ecs::SphereTransformComponent transform;
  transform.position = position;
  transform.rotation = rotation;
  transform.radius = transformData.value("radius", 1.0f);
  ecsManager->addComponent(entity, transform);
}

void jsonfile::Load::createCylinderTransform(
    Entity entity, const raylib::Vector3& position,
    const raylib::Vector3& rotation, const nlohmann::json& transformData,
    ecs::ECSManager* ecsManager) {
  ecs::CylinderTransformComponent transform;
  transform.position = position;
  transform.rotation = rotation;
  transform.radius = transformData.value("radius", 1.0f);
  transform.height = transformData.value("height", 2.0f);
  ecsManager->addComponent(entity, transform);
}

void jsonfile::Load::createPlaneTransform(Entity entity,
                                          const raylib::Vector3& position,
                                          const raylib::Vector3& rotation,
                                          const nlohmann::json& transformData,
                                          ecs::ECSManager* ecsManager) {
  ecs::PlaneTransformComponent transform;
  transform.position = position;
  transform.rotation = rotation;
  transform.size = getVector2FromJson(transformData, "size", {10, 10});
  ecsManager->addComponent(entity, transform);
}

void jsonfile::Load::loadRenderComponent(const nlohmann::json& entityData,
                                         Entity entity,
                                         ecs::ECSManager* ecsManager) {
  if (!entityData.contains("render")) {
    throw std::runtime_error("Entity data does not contain 'render' component");
  }
  const auto& renderData = entityData["render"];
  ecs::RenderComponent render;
  render.color =
      raylib::Color(renderData["color"]["r"], renderData["color"]["g"],
                    renderData["color"]["b"], renderData["color"]["a"]);
  render.visible = renderData.value("visible", true);
  ecsManager->addComponent(entity, render);
}

void jsonfile::Load::loadShapeComponent(Entity entity, ecs::ShapeType shapeType,
                                        ecs::ECSManager* ecsManager) {
  ecs::ShapeComponent shape;
  shape.type = shapeType;
  ecsManager->addComponent(entity, shape);
}

void jsonfile::Load::loadNameComponent(const nlohmann::json& entityData,
                                       Entity entity,
                                       ecs::ECSManager* ecsManager) {
  ecs::NameComponent name;
  if (entityData.contains("name")) {
    const auto& nameData = entityData["name"];
    name.name = nameData.value("name", "Entity");
    name.type = nameData.value("type", "Unknown");
  } else {
    name.name = "Entity";
    name.type = "Unknown";
  }
  ecsManager->addComponent(entity, name);
}

void jsonfile::Load::loadDefaultComponents(Entity entity,
                                           ecs::ECSManager* ecsManager) {
  ecs::SelectionComponent selection;
  selection.selected = false;
  ecsManager->addComponent(entity, selection);

  ecs::GizmoComponent gizmo;
  ecsManager->addComponent(entity, gizmo);
}

raylib::Vector3 jsonfile::Load::getVector3FromJson(
    const nlohmann::json& data, const std::string& key,
    const raylib::Vector3& defaultValue) {
  if (data.contains(key)) {
    const auto& vecData = data[key];
    return raylib::Vector3(vecData.value("x", defaultValue.x),
                           vecData.value("y", defaultValue.y),
                           vecData.value("z", defaultValue.z));
  }
  return defaultValue;
}

raylib::Vector2 jsonfile::Load::getVector2FromJson(
    const nlohmann::json& data, const std::string& key,
    const raylib::Vector2& defaultValue) {
  if (data.contains(key)) {
    const auto& vecData = data[key];
    return raylib::Vector2(vecData.value("x", defaultValue.x),
                           vecData.value("y", defaultValue.y));
  }
  return defaultValue;
}
