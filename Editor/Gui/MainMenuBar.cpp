#include "MainMenuBar.hpp"
#include <iostream>
#include "Application.hpp"
#include "GizmoComponent.hpp"
#include "Gui.hpp"
#include "NameComponent.hpp"
#include "SelectionComponent.hpp"
#include "Shape.hpp"

void ui::MainMenuBar::draw(app::Application &app, ui::Gui &gui) {
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("New", "Ctrl+N")) {
        app.getScene().clearEntities();
        std::cout << "New scene created!" << std::endl;
      }
      if (ImGui::MenuItem("Open", "Ctrl+O")) {
        try {
          app.getScene().load("./scene.json");
          std::cout << "Scene loaded successfully!" << std::endl;
        } catch (const std::exception &e) {
          std::cerr << "Error loading scene: " << e.what() << std::endl;
        }
      }
      if (ImGui::MenuItem("Save", "Ctrl+S")) {
        try {
          app.getScene().save("./scene.json");
          std::cout << "Scene saved successfully!" << std::endl;
        } catch (const std::exception &e) {
          std::cerr << "Error saving scene: " << e.what() << std::endl;
        }
      }
      ImGui::Separator();
      if (ImGui::MenuItem("Exit")) {
        app.setRunning(false);
      }
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("View")) {
      ImGui::MenuItem("Hierarchy", nullptr, &gui._showHierarchy);
      ImGui::MenuItem("Properties", nullptr, &gui._showProperties);
      ImGui::MenuItem("Finder", nullptr, &gui._showFinder);
      ImGui::Separator();
      if (ImGui::BeginMenu("Panel Visibility")) {
        ImGui::MenuItem("Entity Info", nullptr, &gui._showEntities);
        ImGui::MenuItem("Transform Info", nullptr, &gui._showTransform);
        ImGui::MenuItem("Materials Info", nullptr, &gui._showMaterials);
        ImGui::MenuItem("Scripts Info", nullptr, &gui._showScripts);
        ImGui::MenuItem("Camera Info", nullptr, &gui._showCamera);
        ImGui::EndMenu();
      }
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Create")) {
      shape3D::Shape shape;
      if (ImGui::MenuItem("Cube")) {
        shape.createCubeEntity({0.0f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f},
                               raylib::Color::White(), "New Cube",
                               &app.getECSManager());
      }
      if (ImGui::MenuItem("Sphere")) {
        shape.createSphereEntity({0.0f, 0.5f, 0.0f}, 1.0f,
                                 raylib::Color::White(), "New Sphere",
                                 &app.getECSManager());
      }
      if (ImGui::MenuItem("Cylinder")) {
        shape.createCylinderEntity({0.0f, 0.5f, 0.0f}, 1.0f, 2.0f,
                                   raylib::Color::White(), "New Cylinder",
                                   &app.getECSManager());
      }
      if (ImGui::MenuItem("Plane")) {
        shape.createPlaneEntity({0.0f, 0.0f, 0.0f}, {10.0f, 10.0f},
                                raylib::Color::White(), "New Plane",
                                &app.getECSManager());
      }
      ImGui::Separator();
      if (ImGui::MenuItem("Camera")) {
        Entity cameraEntity = app.getECSManager().createEntity();

        ecs::CameraComponent cameraComponent;
        cameraComponent.position = raylib::Vector3(0.0f, 2.0f, 5.0f);
        cameraComponent.target = raylib::Vector3(0.0f, 0.0f, 0.0f);
        cameraComponent.up = raylib::Vector3(0.0f, 1.0f, 0.0f);
        cameraComponent.fov = 45.0f;
        cameraComponent.isActive = false;
        app.getECSManager().addComponent(cameraEntity, cameraComponent);

        ecs::NameComponent nameComp{"New Camera", "Camera"};
        app.getECSManager().addComponent(cameraEntity, nameComp);

        ecs::SelectionComponent selectionComp{false};
        app.getECSManager().addComponent(cameraEntity, selectionComp);

        ecs::GizmoComponent gizmoComp{};
        app.getECSManager().addComponent(cameraEntity, gizmoComp);
      }
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }
}
