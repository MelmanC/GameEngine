#pragma once
#include <raylib.h>
#include <raylib-cpp.hpp>
#include "Vector3.hpp"

namespace shape {

  class GameShape {
    public:
      GameShape(float width, float height, float depth,
                const raylib::Vector3& position);

      ~GameShape() = default;

      void draw() const;

      raylib::Vector3 getPosition() const;
      raylib::Vector3 getSize() const;

      float getWidth() const;
      float getHeight() const;
      float getDepth() const;

    protected:
      raylib::Vector3 _position;
      raylib::Vector3 _size;
      raylib::Color _color;
  };

}  // namespace shape
