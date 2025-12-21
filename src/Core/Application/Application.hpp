#pragma once

#include "../../Platform/WindowSDL.hpp"
#include "../../Renderer/BgfxContext.hpp"
#include "../TimeManager/TimeManager.hpp"

namespace Krio {
    class Application {
        public:
            Application();
            ~Application();

            Application(const Application&) = delete;
            Application& operator=(const Application&) = delete;

            bool initialize();
            void run();
            void shutdown();

        private:
            WindowSDL _window;
            BgfxContext _renderer;
            bool _running;
            TimeManager _timeManager;

            static Application* _instance;

            static void onWindowResize(int width, int height);
            void handleEvents();
            void update();
            void render();
    };
}