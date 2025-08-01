#pragma once

namespace ecs {
  class ECSManager;

  class System {
    public:
      System(ECSManager *manager) : _manager(manager) {
      }

      virtual ~System() = default;

      virtual void update() = 0;

    protected:
      ECSManager *_manager;
  };
}  // namespace ecs
