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

      void setPosition(const raylib::Vector3& position) override {
        _position = position;
      }

      void setColor(const raylib::Color& color) override {
        _color = color;
      }

      void setSize(const raylib::Vector3& size) override {
        _size = size;
      }

      bool isSelected() const override {
        return _isSelected;
      }

      void setName(const std::string& name) override {
        _name = name;
      }
      std::string getType() const override {
        return _type;
      }
      void setType(const std::string& type) override {
        _type = type;
      }
      bool isVisible() const override {
        return _visible;
      }
      void setVisible(bool visible) override {
        _visible = visible;
      }

      raylib::BoundingBox getBoundingBox() const override {
        return raylib::BoundingBox(_position - _size / 2.0f,
                                   _position + _size / 2.0f);
      }

    protected:
      raylib::Vector3 _position;
      raylib::Vector3 _size;
      raylib::Color _color;
      std::string _name;
      bool _isSelected = false;
      std::string _type;
      bool _visible = true;
  };

}  // namespace shape
