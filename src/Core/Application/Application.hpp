#pragma once

#include "../../Platform/WindowSDL.hpp"
#include "../../Renderer/BgfxContext.hpp"
#include "../ServiceLocator/ServiceLocator.hpp"
#include "../TimeManager/TimeManager.hpp"

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

            bool _initializeManagers();
            bool _shutdownManagers();

            void handleEvents();
            void update();
            void render();
    };
}