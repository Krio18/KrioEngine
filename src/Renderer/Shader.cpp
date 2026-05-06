#include "Shader.hpp"

namespace Krio {
    Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
        this->_programHandle = BGFX_INVALID_HANDLE;

        bgfx::ShaderHandle vertexHandle = this->_loadShader(vertexPath);
        if (!bgfx::isValid(vertexHandle)) return;

        bgfx::ShaderHandle fragmentHandle = this->_loadShader(fragmentPath);
        if (!bgfx::isValid(fragmentHandle)) {
            bgfx::destroy(vertexHandle);
            return;
        }

        this->_programHandle = bgfx::createProgram(vertexHandle, fragmentHandle, true);

        if (!bgfx::isValid(this->_programHandle)) {
            Logger::error("Failed to create shader program.");
            bgfx::destroy(vertexHandle);
            bgfx::destroy(fragmentHandle);
            return;
        }
    }

    Shader::~Shader() {
        if (bgfx::isValid(this->_programHandle)) {
            bgfx::destroy(this->_programHandle);
        }
    }

    bgfx::ProgramHandle Shader::getProgramHandle() const {
        return this->_programHandle;
    }

    bgfx::ShaderHandle Shader::_loadShader(const std::string& binShaderPath) {
        std::ifstream binaryFile;
        binaryFile.open(binShaderPath, std::ios::binary);
        if (!binaryFile.is_open()) {
            Logger::error("Failed to open shader: " + binShaderPath);
            return BGFX_INVALID_HANDLE;
        }

        binaryFile.seekg(0, std::ios::end);
        std::streampos endPos = binaryFile.tellg();
        size_t fileSize = static_cast<size_t>(endPos);
        binaryFile.seekg(0, std::ios::beg);

        const bgfx::Memory* buffer = bgfx::alloc(fileSize + 1);
        binaryFile.read((char *)buffer->data, fileSize);

        buffer->data[fileSize] = '\0';

        binaryFile.close();

        bgfx::ShaderHandle tmpShader = bgfx::createShader(buffer);

        if (bgfx::isValid(tmpShader)) {
            return tmpShader;
        } else {
            Logger::error("Failed to open shader: " + binShaderPath);
            return BGFX_INVALID_HANDLE;
        }
    }
}
