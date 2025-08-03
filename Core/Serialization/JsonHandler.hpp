#pragma once

#include "Load.hpp"
#include "Save.hpp"

namespace jsonfile {

  class JsonHandler {
    public:
      JsonHandler() = default;
      ~JsonHandler() = default;

      void save(const scene::Scene& scene, const std::string& jsonFilePath,
                ecs::ECSManager* ecsManager) const {
        Save saver;
        saver.saveToJson(scene, jsonFilePath, ecsManager);
      }

      void load(scene::Scene& scene, const std::string& jsonFilePath,
                ecs::ECSManager* ecsManager) const {
        Load loader;
        loader.loadFromJson(scene, jsonFilePath, ecsManager);
      }
  };
}  // namespace jsonfile
