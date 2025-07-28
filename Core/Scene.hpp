#pragma once
#include <raylib.h>
#include <raylib-cpp.hpp>
#include "IGameShape.hpp"

namespace scene {
  class Scene {
    public:
      std::vector<std::unique_ptr<shape::IGameShape>> _shapes;

      Scene() = default;
      ~Scene() = default;

      void loadFromJson(const std::string& jsonFilePath);
      void saveToJson(const std::string& jsonFilePath) const;
      void draw() const;
      void setSelectedObject(int index);
      void addShape(std::unique_ptr<shape::IGameShape> shape);
      shape::IGameShape* getSelectedObject() const;
  };
}  // namespace scene
