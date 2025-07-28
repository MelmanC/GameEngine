#pragma once
#include <raylib.h>
#include <raylib-cpp.hpp>
#include "AGameShape.hpp"
#include "Vector3.hpp"

namespace shape {

  class Cube : public AGameShape {
    public:
      Cube(float width, float height, float depth,
           const raylib::Vector3& position);

      void draw() const override;

    protected:
  };

}  // namespace shape
