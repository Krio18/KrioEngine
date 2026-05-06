#pragma once

#include "../../Platform/WindowSDL.hpp"
#include "../../Renderer/BgfxContext.hpp"
#include "../ServiceLocator/ServiceLocator.hpp"
#include "../TimeManager/TimeManager.hpp"
#include "../InputManager/InputManager.hpp"
#include "../PhysicsManager/PhysicsManager.hpp"
#include "../SceneManager/SceneManager.hpp"
#include "../RenderManager/RenderManager.hpp"
#include "../../Renderer/Mesh.hpp"
#include "../../Renderer/ShaderManager.hpp"

namespace Krio {
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