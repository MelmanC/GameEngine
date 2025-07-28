#pragma once

#include <raylib.h>
#include <raylib-cpp.hpp>
#include "IGameShape.hpp"
#include "Vector3.hpp"

namespace shape {
  class AGameShape : public IGameShape {
    public:
      virtual ~AGameShape() = default;

      void draw() const override = 0;

      raylib::Vector3 getPosition() const override {
        return _position;
      }

      raylib::Vector3 getSize() const override {
        return _size;
      }

      float getWidth() const override {
        return _size.x;
      }

      float getHeight() const override {
        return _size.y;
      }

      float getDepth() const override {
        return _size.z;
      }

      raylib::Color getColor() const override {
        return _color;
      }

      std::string getName() const override {
        return _name;
      }

      void setSelected(bool isSelected) override {
        _isSelected = isSelected;
      }

      bool isSelected() const override {
        return _isSelected;
      }

    protected:
      raylib::Vector3 _position;
      raylib::Vector3 _size;
      raylib::Color _color;
      std::string _name;
      bool _isSelected = false;
  };

}  // namespace shape
