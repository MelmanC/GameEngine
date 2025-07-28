#pragma once
#include <raylib.h>
#include <raylib-cpp.hpp>
#include "GameShape.hpp"

namespace scene {
  class Scene {
    public:
      std::vector<shape::GameShape> _shapes;

      Scene() = default;
      ~Scene() = default;

      void loadFromJson(const std::string& jsonFilePath);
      void saveToJson(const std::string& jsonFilePath) const;
      void draw() const;
      void setSelectedObject(int index);
  };
}  // namespace scene
