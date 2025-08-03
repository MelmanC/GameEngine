#pragma once

#include "System.hpp"

namespace ecs {
  class ECSManager;

  class SelectionSystem : public System {
    public:
      SelectionSystem() = default;

      void setECSManager(ECSManager* ecsManager) {
        _ecsManager = ecsManager;
      }

      void update(float deltaTime) override;

      void selectEntity(Entity entity);

      void deselectAll();

      Entity getSelectedEntity() const;

      bool hasSelection() const;

    private:
      ECSManager* _ecsManager = nullptr;
      Entity _selectedEntity = 0;
      bool _hasSelection = false;
  };

}  // namespace ecs
