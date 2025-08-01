#pragma once

#include "Color.hpp"
#include "Component.hpp"

namespace ecs {
  class RenderComponent : public Component<RenderComponent> {
    public:
      RenderComponent(const raylib::Color &color = raylib::Color::White(),
                      bool visible = true)
          : _color(color), _visible(visible) {
      }

      const raylib::Color &getColor() const {
        return _color;
      }

      bool isVisible() const {
        return _visible;
      }

      void setColor(const raylib::Color &color) {
        _color = color;
      }

      void setVisible(bool visible) {
        _visible = visible;
      }

    private:
      raylib::Color _color;
      bool _visible;
  };

}  // namespace ecs
