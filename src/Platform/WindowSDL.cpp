#include "WindowSDL.hpp"
#include "../Core/Logger.hpp"

namespace Krio {
    WindowSDL::WindowSDL()
        : _window(nullptr)
        , _shouldClose(false)
        , _width(0)
        , _height(0)
        , _nativeWindowHandle(nullptr)
        , _nativeDisplayHandle(nullptr)
        , _subsystemType(SDL_SYSWM_UNKNOWN)
        , _resizeCallback(nullptr)
    {}

    WindowSDL::~WindowSDL()
    {
        if (this->_window)
            SDL_DestroyWindow(this->_window);
        SDL_Quit();
    }

    bool WindowSDL::create(const std::string& title, int width, int height)
    {
        this->_width = width;
        this->_height = height;

        Logger::info("Initializing SDL2...");
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            Logger::error("SDL_Init failed: " + std::string(SDL_GetError()));
            return false;
        }

        this->_window = SDL_CreateWindow(
            title.c_str(),
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            this->_width, this->_height,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
        );

        if (!this->_window) {
            Logger::error("SDL_CreateWindow failed: " + std::string(SDL_GetError()));
            SDL_Quit();
            return false;
        }

        Logger::info("Window created: " + std::to_string(width) + "x" + std::to_string(height));

        if (!retrieveNativeHandles()) {
            Logger::error("Failed to retrieve native window handles");
            return false;
        }

        return true;
    }

    bool WindowSDL::retrieveNativeHandles()
    {
        SDL_SysWMinfo wmi;
        SDL_VERSION(&wmi.version);

        if (!SDL_GetWindowWMInfo(this->_window, &wmi))
            return false;

        this->_subsystemType = wmi.subsystem;

        if (this->_subsystemType == SDL_SYSWM_X11) {
            this->_nativeDisplayHandle = wmi.info.x11.display;
            this->_nativeWindowHandle = (void*)(uintptr_t)wmi.info.x11.window;
        }
        else if (this->_subsystemType == SDL_SYSWM_WAYLAND) {
            this->_nativeDisplayHandle = wmi.info.wl.display;
            this->_nativeWindowHandle = wmi.info.wl.surface;
        }
    #ifdef _WIN32
        else if (this->_subsystemType == SDL_SYSWM_WINDOWS) {
            this->_nativeWindowHandle = wmi.info.win.window;
        }
    #endif
    #ifdef __APPLE__
        else if (this->_subsystemType == SDL_SYSWM_COCOA) {
            this->_nativeWindowHandle = wmi.info.cocoa.window;
        }
    #endif
        else
            return false;

        if (this->_nativeWindowHandle == nullptr)
            return false;

        return true;
    }

    void WindowSDL::pollEvents()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    this->_shouldClose = true;
                    break;

                case SDL_WINDOWEVENT:
                    if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                        this->_width = event.window.data1;
                        this->_height = event.window.data2;

                        if (this->_resizeCallback)
                            this->_resizeCallback(this->_width, this->_height);
                    }
                    break;

                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                        this->_shouldClose = true;
                    break;
            }
        }
    }

    bool WindowSDL::shouldClose() const
    {
        return this->_shouldClose;
    }

    void* WindowSDL::getNativeWindowHandle() const
    {
        return this->_nativeWindowHandle;
    }

    void* WindowSDL::getNativeDisplayHandle() const
    {
        return this->_nativeDisplayHandle;
    }

    SDL_SYSWM_TYPE WindowSDL::getSubsystemType() const
    {
        return this->_subsystemType;
    }

    int WindowSDL::getWidth() const
    {
        return this->_width;
    }

    int WindowSDL::getHeight() const
    {
        return this->_height;
    }

    void WindowSDL::setResizeCallback(void (*callback)(int, int))
    {
        this->_resizeCallback = callback;
    }
}
