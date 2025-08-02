#pragma once

#include "BoundingBox.hpp"
#include "Vector3.hpp"

namespace ecs {

  struct TransformComponent {
      raylib::Vector3 position{0, 0, 0};
      raylib::Vector3 size{1, 1, 1};

      raylib::BoundingBox getBoundingBox() const {
        return raylib::BoundingBox{position - size / 2, position + size / 2};
      }
  };
}  // namespace ecs
