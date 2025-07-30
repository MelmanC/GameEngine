#pragma once

#include <nlohmann/json.hpp>

namespace scene {
  class Scene;
}

namespace jsonfile {

  class JsonHandler {
    public:
      JsonHandler() = default;
      ~JsonHandler() = default;

      void loadFromJson(scene::Scene& scene, const std::string& jsonFilePath);
      void saveToJson(const scene::Scene& scene,
                      const std::string& jsonFilePath) const;
  };
}  // namespace jsonfile
