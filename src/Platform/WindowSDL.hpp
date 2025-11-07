#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <string>

namespace Krio {
    class WindowSDL {
        public:
            WindowSDL();
            ~WindowSDL();

            WindowSDL(const WindowSDL&) = delete;
            WindowSDL& operator=(const WindowSDL&) = delete;

            bool create(const std::string& title, int width, int height);
            void pollEvents();
            bool shouldClose() const;
            void* getNativeWindowHandle() const;
            void* getNativeDisplayHandle() const;
            SDL_SYSWM_TYPE getSubsystemType() const;
            int getWidth() const;
            int getHeight() const;
            void setResizeCallback(void (*callback)(int, int));

        private:
            SDL_Window* _window;
            bool _shouldClose;
            int _width;
            int _height;
            void* _nativeWindowHandle;
            void* _nativeDisplayHandle;
            SDL_SYSWM_TYPE _subsystemType;
            void (*_resizeCallback)(int, int);

            bool retrieveNativeHandles();
    };
}