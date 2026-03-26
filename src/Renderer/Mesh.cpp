#include "Mesh.hpp"

namespace Krio
{
    Mesh::~Mesh() {
        if (bgfx::isValid(this->_vbh)) {
            bgfx::destroy(this->_vbh);
        }

        if (bgfx::isValid(this->_ibh)) {
            bgfx::destroy(this->_ibh);
        }
    }

    bgfx::VertexLayout Mesh::_vertexLayout;

    void Mesh::init() {
        _vertexLayout
            .begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
            .end();
    }
}

