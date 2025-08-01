#pragma once

#include <string>
#include "Component.hpp"

namespace ecs {

  class NameComponent : public Component<NameComponent> {
    public:
      NameComponent(const std::string &name = "Entity",
                    const std::string &type = "Unknown")
          : _name(name), _type(type) {
      }

      const std::string &getName() const {
        return _name;
      }

      void setName(const std::string &name) {
        _name = name;
      }

      const std::string &getType() const {
        return _type;
      }

      void setType(const std::string &type) {
        _type = type;
      }

    private:
      std::string _name;
      std::string _type;
  };

}  // namespace ecs
