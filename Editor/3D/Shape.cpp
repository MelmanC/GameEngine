#include "Shape.hpp"
#include "ECSManager.hpp"
#include "GizmoComponent.hpp"
#include "ModelComponent.hpp"
#include "NameComponent.hpp"
#include "RenderComponent.hpp"
#include "SelectionComponent.hpp"
#include "ShapeComponent.hpp"
#include "TransformComponent.hpp"
#include "Vector2.hpp"

Entity shape3D::Shape::createCubeEntity(const raylib::Vector3& position,
                                        const raylib::Vector3& size,
                                        const raylib::Color& color,
                                        const std::string& name,
                                        ecs::ECSManager* ecsManager) {
  Entity entity = ecsManager->createEntity();

  ecs::CubeTransformComponent transform;
  transform.position = position;
  transform.size = size;

  ecsManager->addComponent(entity, transform);
  ecsManager->addComponent(entity, ecs::RenderComponent{color, true});
  ecsManager->addComponent(entity, ecs::ShapeComponent{ecs::ShapeType::CUBE});
  ecsManager->addComponent(entity, ecs::NameComponent{name, "Cube"});
  ecsManager->addComponent(entity, ecs::SelectionComponent{false});
  ecsManager->addComponent(entity, ecs::GizmoComponent{});

  return entity;
}

Entity shape3D::Shape::createSphereEntity(const raylib::Vector3& position,
                                          float radius,
                                          const raylib::Color& color,
                                          const std::string& name,
                                          ecs::ECSManager* ecsManager) {
  Entity entity = ecsManager->createEntity();

  ecs::SphereTransformComponent transform;
  transform.position = position;
  transform.radius = radius;

  ecsManager->addComponent(entity, transform);
  ecsManager->addComponent(entity, ecs::RenderComponent{color, true});
  ecsManager->addComponent(entity, ecs::ShapeComponent{ecs::ShapeType::SPHERE});
  ecsManager->addComponent(entity, ecs::NameComponent{name, "Sphere"});
  ecsManager->addComponent(entity, ecs::SelectionComponent{false});
  ecsManager->addComponent(entity, ecs::GizmoComponent{});

  return entity;
}

Entity shape3D::Shape::createCylinderEntity(const raylib::Vector3& position,
                                            float radius, float height,
                                            const raylib::Color& color,
                                            const std::string& name,
                                            ecs::ECSManager* ecsManager) {
  Entity entity = ecsManager->createEntity();

  ecs::CylinderTransformComponent transform;
  transform.position = position;
  transform.radius = radius;
  transform.height = height;

  ecsManager->addComponent(entity, transform);
  ecsManager->addComponent(entity, ecs::RenderComponent{color, true});
  ecsManager->addComponent(entity,
                           ecs::ShapeComponent{ecs::ShapeType::CYLINDER});
  ecsManager->addComponent(entity, ecs::NameComponent{name, "Cylinder"});
  ecsManager->addComponent(entity, ecs::SelectionComponent{false});
  ecsManager->addComponent(entity, ecs::GizmoComponent{});

  return entity;
}

Entity shape3D::Shape::createPlaneEntity(const raylib::Vector3& position,
                                         raylib::Vector2 size,
                                         const raylib::Color& color,
                                         const std::string& name,
                                         ecs::ECSManager* ecsManager) {
  Entity entity = ecsManager->createEntity();
  ecs::PlaneTransformComponent transform;
  transform.position = position;
  transform.size = size;
  ecsManager->addComponent(entity, transform);
  ecsManager->addComponent(entity, ecs::RenderComponent{color, true});
  ecsManager->addComponent(entity, ecs::ShapeComponent{ecs::ShapeType::PLANE});
  ecsManager->addComponent(entity, ecs::NameComponent{name, "Plane"});
  ecsManager->addComponent(entity, ecs::SelectionComponent{false});
  ecsManager->addComponent(entity, ecs::GizmoComponent{});

  return entity;
}

Entity shape3D::Shape::createModelEntity(const raylib::Vector3& position,
                                         const raylib::Vector3& rotation,
                                         const std::string& modelPath,
                                         const raylib::Color& color,
                                         const std::string& name,
                                         ecs::ECSManager* ecsManager) {
  Entity entity = ecsManager->createEntity();

  ecs::ModelTransformComponent transform;
  transform.position = position;
  transform.rotation = rotation;

  ecsManager->addComponent(entity, transform);
  ecsManager->addComponent(entity, ecs::RenderComponent{color, true});
  ecsManager->addComponent(entity, ecs::ShapeComponent{ecs::ShapeType::MODEL});
  ecsManager->addComponent(entity, ecs::NameComponent{name, "Model"});
  ecsManager->addComponent(entity, ecs::SelectionComponent{false});
  ecsManager->addComponent(entity, ecs::GizmoComponent{});

  ecs::ModelComponent modelComponent;
  modelComponent.modelPath = modelPath;
  modelComponent.isLoaded = false;

  ecsManager->addComponent(entity, std::move(modelComponent));

  return entity;
}
