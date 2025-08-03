#pragma once

#include <nlohmann/json.hpp>

namespace scene {
  class Scene;
}

namespace ecs {
  class ECSManager;
}

namespace jsonfile {

  class JsonHandler {
    public:
      JsonHandler() = default;
      ~JsonHandler() = default;

      void loadFromJson(scene::Scene& scene, const std::string& jsonFilePath,
                        ecs::ECSManager* ecsManager);
      void saveToJson(const scene::Scene& scene,
                      const std::string& jsonFilePath,
                      ecs::ECSManager* ecsManager) const;
  };
}  // namespace jsonfile
