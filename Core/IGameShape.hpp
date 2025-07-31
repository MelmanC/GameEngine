#pragma once

#include <raylib.h>
#include <raylib-cpp.hpp>
#include "Vector3.hpp"

namespace shape {

  class IGameShape {
    public:
      virtual ~IGameShape() = default;

      virtual void draw() const = 0;

      virtual raylib::Vector3 getPosition() const = 0;
      virtual raylib::Vector3 getSize() const = 0;

      virtual float getWidth() const = 0;
      virtual float getHeight() const = 0;
      virtual float getDepth() const = 0;
      virtual raylib::Color getColor() const = 0;
      virtual std::string getName() const = 0;
      virtual void setSelected(bool isSelected) = 0;
      virtual void setPosition(const raylib::Vector3& position) = 0;
      virtual void setColor(const raylib::Color& color) = 0;
      virtual void setSize(const raylib::Vector3& size) = 0;
      virtual void setName(const std::string& name) = 0;
      virtual std::string getType() const = 0;
      virtual void setType(const std::string& type) = 0;
      virtual bool isVisible() const = 0;
      virtual void setVisible(bool visible) = 0;
      virtual raylib::BoundingBox getBoundingBox() const = 0;

      virtual bool isSelected() const = 0;
  };

}  // namespace shape
