#include "Scene.hpp"
#include <fstream>
#include <nlohmann/json.hpp>

void scene::Scene::loadFromJson(const std::string& jsonFilePath) {
  std::ifstream file(jsonFilePath);
  if (!file.is_open()) {
    throw std::runtime_error("Could not open JSON file: " + jsonFilePath);
  }
  nlohmann::json jsonData;
  file >> jsonData;
  _shapes.clear();

  for (const auto& shapeData : jsonData["shapes"]) {
    int width = shapeData["width"];
    int height = shapeData["height"];
    float depth = shapeData.value("depth", 1.0f);
    raylib::Vector3 position(shapeData["position"]["x"],
                             shapeData["position"]["y"],
                             shapeData["position"]["z"]);
    _shapes.push_back(shape::GameShape(width, height, depth, position));
  }
}

void scene::Scene::saveToJson(const std::string& jsonFilePath) const {
  nlohmann::json jsonData;
  for (const auto& shape : _shapes) {
    nlohmann::json shapeData;
    shapeData["width"] = shape.getWidth();
    shapeData["height"] = shape.getHeight();
    shapeData["depth"] = shape.getDepth();
    shapeData["position"]["x"] = shape.getPosition().x;
    shapeData["position"]["y"] = shape.getPosition().y;
    shapeData["position"]["z"] = shape.getPosition().z;
    jsonData["shapes"].push_back(shapeData);
  }

  std::ofstream file(jsonFilePath);
  if (!file.is_open()) {
    throw std::runtime_error("Could not open JSON file for writing: " +
                             jsonFilePath);
  }
  file << jsonData.dump(4);
}

void scene::Scene::draw() const {
  for (const auto& shape : _shapes) {
    shape.draw();
  }
}
