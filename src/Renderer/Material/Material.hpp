#pragma once

#include <glm/glm.hpp>
#include <bgfx/bgfx.h>
#include <unordered_map>
#include <string>

namespace Krio {
    struct UniformValue {
        glm::vec4 color;
        float metallic;
        float roughness;
    };

    class Material {
        public:
            Material(bgfx::ProgramHandle shaderHandle, const std::string& shaderName);
            Material(const Material& other);

            ~Material();

            void setColor(const glm::vec4& color);
            void setTexture(const std::string& name, bgfx::TextureHandle handle);
            void submit() const;
            bgfx::ProgramHandle getShaderHandle() const;
            static void shutdown();

        private:
            bgfx::ProgramHandle _shaderHandle;
            std::string _shaderName;
            UniformValue _uniformValue;
            std::unordered_map<std::string, bgfx::TextureHandle> _textures;
            std::unordered_map<std::string, bgfx::UniformHandle> _samplerUniforms;

            static bgfx::UniformHandle _uColor;
            static bgfx::UniformHandle _uMaterialParams;
    };
}
