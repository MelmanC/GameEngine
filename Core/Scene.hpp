#pragma once
#include <raylib.h>
#include <raylib-cpp.hpp>
#include "ECSManager.hpp"
#include "EntityManager.hpp"
#include "JsonHandler.hpp"

namespace ecs {
  class SelectionSystem;
}  // namespace ecs

namespace scene {
  class Scene {
    public:
      Scene() = default;
      ~Scene() = default;

      void initialize(ecs::SelectionSystem* selectionSystem,
                      ecs::ECSManager* ecsManager);

      void setSelectedEntity(Entity entity);
      Entity getSelectedEntity() const;
      bool hasSelectedEntity() const;

      void clearEntities();

      std::vector<Entity> getAllEntities() const;

      void save(const std::string& jsonFilePath) const;
      void load(const std::string& jsonFilePath);

    private:
      ecs::SelectionSystem* _selectionSystem = nullptr;
      ecs::ECSManager* _ecsManager = nullptr;
      std::unique_ptr<jsonfile::JsonHandler> _jsonHandler;
  };
}  // namespace scene
