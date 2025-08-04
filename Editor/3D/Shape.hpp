#pragma once

#include <raylib.h>
#include <raylib-cpp.hpp>
#include "ECSManager.hpp"

namespace shape3D {
  class Shape {
    public:
      Shape() = default;
      ~Shape() = default;

      Entity createCubeEntity(const raylib::Vector3& position,
                              const raylib::Vector3& size,
                              const raylib::Color& color,
                              const std::string& name,
                              ecs::ECSManager* ecsManager);

      Entity createSphereEntity(const raylib::Vector3& position, float radius,
                                const raylib::Color& color,
                                const std::string& name,
                                ecs::ECSManager* ecsManager);

      Entity createCylinderEntity(const raylib::Vector3& position, float radius,
                                  float height, const raylib::Color& color,
                                  const std::string& name,
                                  ecs::ECSManager* ecsManager);

      Entity createPlaneEntity(const raylib::Vector3& position,
                               raylib::Vector2 size, const raylib::Color& color,
                               const std::string& name,
                               ecs::ECSManager* ecsManager);

      Entity createModelEntity(const raylib::Vector3& position,
                               const raylib::Vector3& scale,
                               const std::string& modelPath,
                               const raylib::Color& color,
                               const std::string& name,
                               ecs::ECSManager* ecsManager);
  };
}  // namespace shape3D
