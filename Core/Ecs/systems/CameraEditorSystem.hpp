#pragma once

#include <unordered_map>
#include "ECSManager.hpp"
#include "System.hpp"
#include "Vector3.hpp"

namespace ecs {
  class CameraEditorSystem : public System {
    public:
      CameraEditorSystem() = default;

      void setECSManager(ecs::ECSManager* ecsManager) {
        _ecsManager = ecsManager;
      }

      void update(float deltaTime) override;

      Entity createCamera(Entity cameraEntity);

      void removeCamera(Entity cameraEntity);

      void updateVisualization(Entity cameraEntity, Entity visualEntity);

      Entity getCameraFromEditor(Entity visualEntity) const;

    private:
      ecs::ECSManager* _ecsManager = nullptr;

      std::unordered_map<Entity, Entity> _cameraToEditor;

      Entity createCameraShape(const raylib::Vector3& position);
  };
}  // namespace ecs
