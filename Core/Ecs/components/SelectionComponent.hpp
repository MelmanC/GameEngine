#pragma once

#include "Component.hpp"

namespace ecs {

  class SelectionComponent : public Component<SelectionComponent> {
    public:
      SelectionComponent(bool selected = false) : _selected(selected) {
      }

      bool isSelected() const {
        return _selected;
      }

      void setSelected(bool selected) {
        _selected = selected;
      }

    private:
      bool _selected;
  };
}  // namespace ecs
