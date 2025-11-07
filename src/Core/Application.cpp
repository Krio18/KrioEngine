#include "Application.hpp"
#include "Logger.hpp"

namespace Krio {
    Application* Application::_instance = nullptr;

    Application::Application()
        : _running(false)
    {
        _instance = this;
    }

    Application::~Application()
    {
        shutdown();
        _instance = nullptr;
    }

    bool Application::initialize()
    {
        Logger::info("Initializing KrioEngine...");

        if (!this->_window.create("KrioEngine", 1280, 720)) {
            Logger::error("Failed to create window");
            return false;
        }

        if (!this->_renderer.initialize(
                this->_window.getNativeWindowHandle(),
                this->_window.getNativeDisplayHandle(),
                this->_window.getWidth(),
                this->_window.getHeight()
            )) {
            Logger::error("Failed to initialize renderer");
            return false;
        }

        this->_window.setResizeCallback(onWindowResize);
        this->_running = true;

        Logger::info("KrioEngine initialized successfully");
        return true;
    }

    void Application::run()
    {
        this->_lastFrameTime = std::chrono::steady_clock::now();

        while (this->_running) {
            auto now = std::chrono::steady_clock::now();
            float deltaTime = std::chrono::duration<float>(now - this->_lastFrameTime).count();
            this->_lastFrameTime = now;

            handleEvents();
            update(deltaTime);
            render();
        }
    }

    void Application::shutdown()
    {
        this->_running = false;
    }

    void Application::handleEvents()
    {
        this->_window.pollEvents();

        if (this->_window.shouldClose())
            this->_running = false;
    }

    void Application::update(float deltaTime) {}

    void Application::render()
    {
        this->_renderer.frame();
    }

    void Application::onWindowResize(int width, int height)
    {
        if (_instance && _instance->_renderer.isInitialized())
            _instance->_renderer.resize(width, height);
    }
}