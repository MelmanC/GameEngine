#pragma once

#include <raylib.h>
#include <limits>
#include "CameraComponent.hpp"
#include "ECSManager.hpp"
#include "System.hpp"

namespace ecs {

  class CameraSystem : public System {
    public:
      CameraSystem() = default;

      void setECSManager(ECSManager* ecsManager) {
        _ecsManager = ecsManager;
      }

      void update(float deltaTime) override;

      void setActiveCamera(Entity camera);

      bool hasActiveCamera() const {
        return _activeCamera != std::numeric_limits<Entity>::max() &&
               _ecsManager->hasComponent<CameraComponent>(_activeCamera);
      }

      raylib::Camera3D getActiveCamera() const;

    private:
      ECSManager* _ecsManager = nullptr;
      Entity _activeCamera = std::numeric_limits<Entity>::max();
  };

}  // namespace ecs
