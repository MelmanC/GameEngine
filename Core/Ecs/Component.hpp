#pragma once

namespace ecs {

  using ComponentID = unsigned int;

  class BaseComponent {
    public:
      virtual ~BaseComponent() = default;
  };

  template <typename T>
  class Component : public BaseComponent {
    public:
      virtual ~Component() = default;

      static ComponentID getStaticComponentID() {
        static ComponentID id = _nextComponentID++;
        return id;
      }

    private:
      static ComponentID _nextComponentID;
  };

  template <typename T>
  ComponentID Component<T>::_nextComponentID = 1;
}  // namespace ecs
