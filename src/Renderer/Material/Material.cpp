#include "Material.hpp"

namespace Krio {
    bgfx::UniformHandle Material::_uColor = BGFX_INVALID_HANDLE;
    bgfx::UniformHandle Material::_uMaterialParams = BGFX_INVALID_HANDLE;

    Material::Material(bgfx::ProgramHandle shaderHandle, const std::string& shaderName) : _shaderHandle(shaderHandle), _shaderName(shaderName) {
        if (!bgfx::isValid(this->_uColor)) {
            this->_uColor = bgfx::createUniform("u_color", bgfx::UniformType::Vec4);
        }
        if (!bgfx::isValid(this->_uMaterialParams)) {
            this->_uMaterialParams = bgfx::createUniform("u_materialParams", bgfx::UniformType::Vec4);
        }
    }

    Material::Material(const Material& other) : _shaderHandle(other._shaderHandle), _shaderName(other._shaderName), _uniformValue(other._uniformValue), _textures(other._textures) {
        for (const auto& [name, handle] : this->_textures) {
            this->_samplerUniforms[name] = bgfx::createUniform(name.c_str(), bgfx::UniformType::Sampler);
        }
    }

    Material::~Material() {
        for (auto& sampler : this->_samplerUniforms) {
            if (bgfx::isValid(sampler.second)) {
                bgfx::destroy(sampler.second);
            }
        }
    }

    void Material::shutdown() {
        if (bgfx::isValid(Material::_uColor)) {
            bgfx::destroy(Material::_uColor);
            Material::_uColor = BGFX_INVALID_HANDLE;
        }
        if (bgfx::isValid(Material::_uMaterialParams)) {
            bgfx::destroy(Material::_uMaterialParams);
            Material::_uMaterialParams = BGFX_INVALID_HANDLE;
        }
    }

    void Material::setColor(const glm::vec4& color) {
        this->_uniformValue.color = color;
    }

    void Material::setTexture(const std::string& name, bgfx::TextureHandle handle) {
        this->_textures[name] = handle;
        if (this->_samplerUniforms.find(name) == this->_samplerUniforms.end()) {
            this->_samplerUniforms[name] = bgfx::createUniform(name.c_str(), bgfx::UniformType::Sampler);
        }
    }

    void Material::submit() const {
        bgfx::setUniform(this->_uColor, &this->_uniformValue.color);

        const glm::vec4 materialParams = glm::vec4(this->_uniformValue.metallic, this->_uniformValue.roughness, 0.0f, 0.0f);
        bgfx::setUniform(this->_uMaterialParams, &materialParams);

        uint8_t textureUnit = 0;
        for (const auto& [name, handle] : this->_textures) {
            bgfx::setTexture(textureUnit++, this->_samplerUniforms.at(name), handle);
        }
    }

    bgfx::ProgramHandle Material::getShaderHandle() const {
        return this->_shaderHandle;
    }
}