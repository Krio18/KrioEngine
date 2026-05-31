#pragma once

#include "../../Platform/WindowSDL/WindowSDL.hpp"
#include "../../Platform/InputManager/InputManager.hpp"
#include "../../Renderer/BgfxContext/BgfxContext.hpp"
#include "../ServiceLocator/ServiceLocator.hpp"
#include "../TimeManager/TimeManager.hpp"
#include "../PhysicsManager/PhysicsManager.hpp"
#include "../SceneManager/SceneManager.hpp"
#include "../RenderManager/RenderManager.hpp"
#include "../../Renderer/Mesh/Mesh.hpp"
#include "../../Renderer/ShaderManager/ShaderManager.hpp"
#include "../../Renderer/MaterialManager/MaterialManager.hpp"
#include "../Logger/Logger.hpp"
#include "../../Renderer/MeshManager/MeshManager.hpp"
#include "../../Renderer/CameraManager/CameraManager.hpp"
#include "../../Renderer/Controllers/FPSCameraController/FPSCameraController.hpp"

#include <memory>

namespace Voxel {
    class Application {
        public:
            Application();
            ~Application() = default;

            Application(const Application&) = delete;
            Application& operator=(const Application&) = delete;

            bool shutdown();
            bool initialize();

            void run();
        private:
            WindowSDL _window;
            BgfxContext _renderer;
            bool _running;

            ServiceLocator _serviceLocator;

            std::shared_ptr<Camera> _camera;
            std::unique_ptr<FPSCameraController> _fpsController;


            int _lastWindowWidth;
            int _lastWindowHeight;
            double _physicsAccumulator;

            bool _initializeManagers();
            bool _shutdownManagers();

            void handleEvents();
            void update();
            void render();
    };
}