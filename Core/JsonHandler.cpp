#include "JsonHandler.hpp"
#include <raylib.h>
#include <fstream>
#include <nlohmann/json.hpp>
#include <raylib-cpp.hpp>
#include "Cube.hpp"
#include "Scene.hpp"

void jsonfile::JsonHandler::loadFromJson(scene::Scene& scene,
                                         const std::string& jsonFilePath) {
  std::ifstream file(jsonFilePath);
  if (!file.is_open()) {
    throw std::runtime_error("Could not open JSON file: " + jsonFilePath);
  }
  nlohmann::json jsonData;
  file >> jsonData;
  scene.clearShapes();

  for (const auto& shapeData : jsonData["shapes"]) {
    int width = shapeData["width"];
    int height = shapeData["height"];
    float depth = shapeData.value("depth", 1.0f);
    raylib::Vector3 position(shapeData["position"]["x"],
                             shapeData["position"]["y"],
                             shapeData["position"]["z"]);
    raylib::Color color(shapeData["color"]["r"], shapeData["color"]["g"],
                        shapeData["color"]["b"], shapeData["color"]["a"]);
    std::string name = shapeData.value("name", "Shape");
    std::string type = shapeData.value("type", "Unknown");
    if (type == "Cube") {
      auto cube = std::make_unique<shape::Cube>(width, height, depth, position);
      cube->setColor(color);
      cube->setName(name);
      scene.addShape(std::move(cube));
    }
  }
}

void jsonfile::JsonHandler::saveToJson(const scene::Scene& scene,
                                       const std::string& jsonFilePath) const {
  nlohmann::json jsonData;
  jsonData["shapes"] = nlohmann::json::array();
  for (const auto& shape : scene.getShapes()) {
    nlohmann::json shapeData;
    shapeData["width"] = shape->getWidth();
    shapeData["height"] = shape->getHeight();
    shapeData["depth"] = shape->getDepth();
    shapeData["position"]["x"] = shape->getPosition().x;
    shapeData["position"]["y"] = shape->getPosition().y;
    shapeData["position"]["z"] = shape->getPosition().z;
    shapeData["color"] = {{"r", shape->getColor().r},
                          {"g", shape->getColor().g},
                          {"b", shape->getColor().b},
                          {"a", shape->getColor().a}};
    shapeData["name"] = shape->getName();
    shapeData["type"] = shape->getType();
    jsonData["shapes"].push_back(shapeData);
  }

  std::ofstream file(jsonFilePath);
  if (!file.is_open()) {
    throw std::runtime_error("Could not open JSON file for writing: " +
                             jsonFilePath);
  }
  file << jsonData.dump(2);
}
