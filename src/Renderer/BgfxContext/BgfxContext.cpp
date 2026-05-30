#include "BgfxContext.hpp"

namespace Voxel {
    BgfxContext::BgfxContext()
        : _initialized(false)
        , _width(0)
        , _height(0)
    {}

    BgfxContext::~BgfxContext()
    {
        shutdown();
    }

    bool BgfxContext::initialize(void* windowHandle, void* displayHandle,
                                uint32_t width, uint32_t height)
    {
        this->_width = width;
        this->_height = height;

        Logger::info("Initializing bgfx renderer...");

        bgfx::PlatformData pd = {};
        pd.nwh = windowHandle;
        pd.ndt = displayHandle;

        bgfx::Init init;
        init.type = bgfx::RendererType::Count;
        init.vendorId = BGFX_PCI_ID_NONE;
        init.resolution.width = this->_width;
        init.resolution.height = this->_height;
        init.resolution.reset = BGFX_RESET_VSYNC;
        init.platformData = pd;

        if (!bgfx::init(init)) {
            Logger::error("bgfx::init failed - check graphics drivers");
            return false;
        }

        const bgfx::Caps* caps = bgfx::getCaps();
        Logger::info("bgfx initialized - Renderer: " + std::string(bgfx::getRendererName(caps->rendererType)));

        this->_initialized = true;
        setupDefaultView(this->_width, this->_height);

        return true;
    }

    void BgfxContext::shutdown()
    {
        if (this->_initialized) {
            bgfx::shutdown();
            this->_initialized = false;
        }
    }

    void BgfxContext::setupDefaultView(uint32_t width, uint32_t height)
    {
        this->_width = width;
        this->_height = height;

        bgfx::setViewClear(0,
            BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH,
            0x303030FF,
            1.0f,
            0
        );
        bgfx::setViewRect(0, 0, 0, uint16_t(this->_width), uint16_t(this->_height));
    }

    void BgfxContext::resize(uint32_t width, uint32_t height)
    {
        this->_width = width;
        this->_height = height;

        bgfx::reset(this->_width, this->_height, BGFX_RESET_VSYNC);
        bgfx::setViewRect(0, 0, 0, uint16_t(this->_width), uint16_t(this->_height));
    }

    void BgfxContext::frame()
    {
        bgfx::touch(0);
        bgfx::frame();
    }

    bool BgfxContext::isInitialized() const
    {
        return this->_initialized;
    }
}
