#pragma once

#include "../Core/Logger/Logger.hpp"

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <cstdint>

namespace Krio {
    class BgfxContext {
        public:
            BgfxContext();
            ~BgfxContext();

            BgfxContext(const BgfxContext&) = delete;
            BgfxContext& operator=(const BgfxContext&) = delete;

            bool initialize(void* windowHandle, void* displayHandle,
                        uint32_t width, uint32_t height);
            void shutdown();
            void setupDefaultView(uint32_t width, uint32_t height);
            void resize(uint32_t width, uint32_t height);
            void frame();
            bool isInitialized() const;

        private:
            bool _initialized;
            uint32_t _width;
            uint32_t _height;
    };
}