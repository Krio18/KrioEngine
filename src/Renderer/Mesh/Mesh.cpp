#include "Mesh.hpp"

namespace Voxel
{

    Mesh& Mesh::operator=(Mesh&& other) noexcept {
        if (bgfx::isValid(_vbh)) bgfx::destroy(_vbh);
        if (bgfx::isValid(_ibh)) bgfx::destroy(_ibh);

        _vbh = other._vbh;
        _ibh = other._ibh;
        _indexCount = other._indexCount;

        other._vbh = BGFX_INVALID_HANDLE;
        other._ibh = BGFX_INVALID_HANDLE;
        other._indexCount = 0;

        return *this;
    }

    Mesh::Mesh() : _vbh(BGFX_INVALID_HANDLE), _ibh(BGFX_INVALID_HANDLE), _indexCount(0) {}

    Mesh::Mesh(Mesh&& other) noexcept {
        _vbh = other._vbh;
        _ibh = other._ibh;
        _indexCount = other._indexCount;
        other._vbh = BGFX_INVALID_HANDLE;
        other._ibh = BGFX_INVALID_HANDLE;
        other._indexCount = 0;
    }

    Mesh::~Mesh() {
        if (bgfx::isValid(this->_vbh)) {
            bgfx::destroy(this->_vbh);
        }

        if (bgfx::isValid(this->_ibh)) {
            bgfx::destroy(this->_ibh);
        }
    }

    void Mesh::draw(bgfx::ViewId viewId, bgfx::ProgramHandle program) const {
        bgfx::setVertexBuffer(0, _vbh);
        if (bgfx::isValid(_ibh))
            bgfx::setIndexBuffer(_ibh);

        bgfx::submit(viewId, program);
    }

    uint32_t Mesh::getIndexCount() const {
        return _indexCount;
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

    Mesh Mesh::createTriangle() {
        Mesh mesh;
        std::vector<Vertex> vertices;
        vertices.push_back(Vertex());
        vertices.push_back(Vertex());
        vertices.push_back(Vertex());

        vertices[0].x = 0.0f; vertices[0].y = 1.0f; vertices[0].z = 0.0f;
        vertices[1].x = -1.0f; vertices[1].y = -1.0f; vertices[1].z = 0.0f;
        vertices[2].x = 1.0f; vertices[2].y = -1.0f; vertices[2].z = 0.0f;

        float minX = std::min({vertices[0].x, vertices[1].x, vertices[2].x});
        float minY = std::min({vertices[0].y, vertices[1].y, vertices[2].y});
        float minZ = std::min({vertices[0].z, vertices[1].z, vertices[2].z});
        float maxX = std::max({vertices[0].x, vertices[1].x, vertices[2].x});
        float maxY = std::max({vertices[0].y, vertices[1].y, vertices[2].y});
        float maxZ = std::max({vertices[0].z, vertices[1].z, vertices[2].z});
        mesh._minX = minX;
        mesh._minY = minY;
        mesh._minZ = minZ;
        mesh._maxX = maxX;
        mesh._maxY = maxY;
        mesh._maxZ = maxZ;

        const bgfx::Memory *refVertex = bgfx::copy(vertices.data(), vertices.size() * sizeof(Vertex));
        bgfx::VertexBufferHandle bufferVertex = bgfx::createVertexBuffer(refVertex, _vertexLayout);
        mesh._vbh = bufferVertex;
        mesh._indexCount = 3;
        return mesh;
    }

    Mesh Mesh::createQuad() {
        Mesh mesh;
        std::vector<uint16_t> indices = {
            2, 1, 0,
            3, 2, 0
        };

        std::vector<Vertex> vertices;
        vertices.push_back(Vertex());
        vertices.push_back(Vertex());
        vertices.push_back(Vertex());
        vertices.push_back(Vertex());

        vertices[0].x = -1.0f; vertices[0].y = 1.0f; vertices[0].z = 0.0f;
        vertices[1].x = -1.0f; vertices[1].y = -1.0f; vertices[1].z = 0.0f;
        vertices[2].x = 1.0f; vertices[2].y = -1.0f; vertices[2].z = 0.0f;
        vertices[3].x = 1.0f; vertices[3].y = 1.0f; vertices[3].z = 0.0f;

        float minX = std::min({vertices[0].x, vertices[1].x, vertices[2].x, vertices[3].x});
        float minY = std::min({vertices[0].y, vertices[1].y, vertices[2].y, vertices[3].y});
        float minZ = std::min({vertices[0].z, vertices[1].z, vertices[2].z, vertices[3].z});
        float maxX = std::max({vertices[0].x, vertices[1].x, vertices[2].x, vertices[3].x});
        float maxY = std::max({vertices[0].y, vertices[1].y, vertices[2].y, vertices[3].y});
        float maxZ = std::max({vertices[0].z, vertices[1].z, vertices[2].z, vertices[3].z});
        mesh._minX = minX;
        mesh._minY = minY;
        mesh._minZ = minZ;
        mesh._maxX = maxX;
        mesh._maxY = maxY;
        mesh._maxZ = maxZ;

        const bgfx::Memory *refVertex = bgfx::copy(vertices.data(), vertices.size() * sizeof(Vertex));
        bgfx::VertexBufferHandle bufferVertex = bgfx::createVertexBuffer(refVertex, _vertexLayout);
        mesh._vbh = bufferVertex;

        const bgfx::Memory *refIndex = bgfx::copy(indices.data(), indices.size() * sizeof(uint16_t));
        bgfx::IndexBufferHandle bufferIndex = bgfx::createIndexBuffer(refIndex);
        mesh._ibh = bufferIndex;
        mesh._indexCount = static_cast<uint32_t>(indices.size());

        return mesh;
    }

    Mesh Mesh::createCube() {
        Mesh mesh;
        std::vector<uint16_t> indices = {
            2, 1, 0, 3, 2, 0,
            6, 5, 4, 7, 6, 4,
            10, 9, 8, 11, 10, 8,
            14, 13, 12, 15, 14, 12,
            18, 17, 16, 19, 18, 16,
            22, 21, 20, 23, 22, 20
        };

        std::vector<Vertex> vertices;
        vertices.resize(24);

        vertices[0].x = -1.0f; vertices[0].y = 1.0f; vertices[0].z = 1.0f; vertices[0].nz = 1.0f;
        vertices[1].x = -1.0f; vertices[1].y = -1.0f; vertices[1].z = 1.0f; vertices[1].nz = 1.0f;
        vertices[2].x = 1.0f; vertices[2].y = -1.0f; vertices[2].z = 1.0f; vertices[2].nz = 1.0f;
        vertices[3].x = 1.0f; vertices[3].y = 1.0f; vertices[3].z = 1.0f; vertices[3].nz = 1.0f;

        vertices[4].x = 1.0f; vertices[4].y = 1.0f; vertices[4].z = -1.0f; vertices[4].nz = -1.0f;
        vertices[5].x = 1.0f; vertices[5].y = -1.0f; vertices[5].z = -1.0f; vertices[5].nz = -1.0f;
        vertices[6].x = -1.0f; vertices[6].y = -1.0f; vertices[6].z = -1.0f; vertices[6].nz = -1.0f;
        vertices[7].x = -1.0f; vertices[7].y = 1.0f; vertices[7].z = -1.0f; vertices[7].nz = -1.0f;

        vertices[8].x = -1.0f; vertices[8].y = 1.0f; vertices[8].z = -1.0f; vertices[8].nx = -1.0f;
        vertices[9].x = -1.0f; vertices[9].y = -1.0f; vertices[9].z = -1.0f; vertices[9].nx = -1.0f;
        vertices[10].x = -1.0f; vertices[10].y = -1.0f; vertices[10].z = 1.0f; vertices[10].nx = -1.0f;
        vertices[11].x = -1.0f; vertices[11].y = 1.0f; vertices[11].z = 1.0f; vertices[11].nx = -1.0f;

        vertices[12].x = 1.0f; vertices[12].y = 1.0f; vertices[12].z = 1.0f; vertices[12].nx = 1.0f;
        vertices[13].x = 1.0f; vertices[13].y = -1.0f; vertices[13].z = 1.0f; vertices[13].nx = 1.0f;
        vertices[14].x = 1.0f; vertices[14].y = -1.0f; vertices[14].z = -1.0f; vertices[14].nx = 1.0f;
        vertices[15].x = 1.0f; vertices[15].y = 1.0f; vertices[15].z = -1.0f; vertices[15].nx = 1.0f;

        vertices[16].x = -1.0f; vertices[16].y = 1.0f; vertices[16].z = -1.0f; vertices[16].ny = 1.0f;
        vertices[17].x = -1.0f; vertices[17].y = 1.0f; vertices[17].z = 1.0f; vertices[17].ny = 1.0f;
        vertices[18].x = 1.0f; vertices[18].y = 1.0f; vertices[18].z = 1.0f; vertices[18].ny = 1.0f;
        vertices[19].x = 1.0f; vertices[19].y = 1.0f; vertices[19].z = -1.0f; vertices[19].ny = 1.0f;

        vertices[20].x = -1.0f; vertices[20].y = -1.0f; vertices[20].z = 1.0f; vertices[20].ny = -1.0f;
        vertices[21].x = -1.0f; vertices[21].y = -1.0f; vertices[21].z = -1.0f; vertices[21].ny = -1.0f;
        vertices[22].x = 1.0f; vertices[22].y = -1.0f; vertices[22].z = -1.0f; vertices[22].ny = -1.0f;
        vertices[23].x = 1.0f; vertices[23].y = -1.0f; vertices[23].z = 1.0f; vertices[23].ny = -1.0f;

        float minX = vertices[0].x, maxX = vertices[0].x;
        float minY = vertices[0].y, maxY = vertices[0].y;
        float minZ = vertices[0].z, maxZ = vertices[0].z;
        for (const auto& v : vertices) {
            minX = std::min(minX, v.x);
            minY = std::min(minY, v.y);
            minZ = std::min(minZ, v.z);
            maxX = std::max(maxX, v.x);
            maxY = std::max(maxY, v.y);
            maxZ = std::max(maxZ, v.z);
        }
        mesh._minX = minX;
        mesh._minY = minY;
        mesh._minZ = minZ;
        mesh._maxX = maxX;
        mesh._maxY = maxY;
        mesh._maxZ = maxZ;

        const bgfx::Memory *refVertex = bgfx::copy(vertices.data(), vertices.size() * sizeof(Vertex));
        bgfx::VertexBufferHandle bufferVertex = bgfx::createVertexBuffer(refVertex, _vertexLayout);
        mesh._vbh = bufferVertex;

        const bgfx::Memory *refIndex = bgfx::copy(indices.data(), indices.size() * sizeof(uint16_t));
        bgfx::IndexBufferHandle bufferIndex = bgfx::createIndexBuffer(refIndex);
        mesh._ibh = bufferIndex;
        mesh._indexCount = static_cast<uint32_t>(indices.size());

        return mesh;
    }
}