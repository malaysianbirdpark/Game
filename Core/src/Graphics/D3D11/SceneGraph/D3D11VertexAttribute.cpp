#include "pch.h"
#include "D3D11VertexAttribute.h"

Engine::x_vector<uint8_t> Engine::Graphics::D3D11VertexAttribute::GetBuffer(aiMesh const* ai_mesh, x_string const& vertex_format) {
    if (!_strideTable.contains(vertex_format)) {
        GRAPHICS_ERROR("Invalid vertex format!");
        return {};
    }

    x_vector<uint8_t> buffer(_strideTable[vertex_format] * ai_mesh->mNumVertices);
    _parseFunctionTable[vertex_format](ai_mesh, buffer);

    return buffer;
}

UINT Engine::Graphics::D3D11VertexAttribute::GetStride(x_string const& vertex_format) {
    if (!_strideTable.contains(vertex_format)) {
        GRAPHICS_ERROR("Invalid vertex format!");
        return 0u;
    }

    return _strideTable[vertex_format];
}

UINT Engine::Graphics::D3D11VertexAttribute::GeyByteWidth(x_string const& vertex_format, size_t number_of_vertices) {
    if (!_strideTable.contains(vertex_format)) {
        GRAPHICS_ERROR("Invalid vertex format!");
        return 0u;
    }

    return number_of_vertices * _strideTable[vertex_format];
}

void Engine::Graphics::D3D11VertexAttribute::ParseVertex_1P(aiMesh const*& ai_mesh, x_vector<uint8_t>& buffer) {
    size_t pos {0u};
    for (auto i {size_t{}}; i != ai_mesh->mNumVertices; ++i) {
        std::copy_n(
            reinterpret_cast<uint8_t*>(&ai_mesh->mVertices[i]),
            sizeof(DirectX::XMFLOAT3),
            buffer.begin() + pos
        );

        pos += sizeof(DirectX::XMFLOAT3);
    }
}

void Engine::Graphics::D3D11VertexAttribute::ParseVertex_1P1N(aiMesh const*& ai_mesh, x_vector<uint8_t>& buffer) {
    size_t pos {0u};
    for (auto i {size_t{}}; i != ai_mesh->mNumVertices; ++i) {
        std::copy_n(
            reinterpret_cast<uint8_t*>(&ai_mesh->mVertices[i]),
            sizeof(DirectX::XMFLOAT3),
            buffer.begin() + pos
        );
        pos += sizeof(DirectX::XMFLOAT3);

        std::copy_n(
            reinterpret_cast<uint8_t*>(&ai_mesh->mNormals[i]),
            sizeof(DirectX::XMFLOAT3),
            buffer.begin() + pos
        );
        pos += sizeof(DirectX::XMFLOAT3);
    }
}

void Engine::Graphics::D3D11VertexAttribute::ParseVertex_1P1N1UV(aiMesh const*& ai_mesh, x_vector<uint8_t>& buffer) {
    size_t pos {0u};
    for (auto i {size_t{}}; i != ai_mesh->mNumVertices; ++i) {
        std::copy_n(
            reinterpret_cast<uint8_t*>(&ai_mesh->mVertices[i]),
            sizeof(DirectX::XMFLOAT3),
            buffer.begin() + pos
        );
        pos += sizeof(DirectX::XMFLOAT3);

        std::copy_n(
            reinterpret_cast<uint8_t*>(&ai_mesh->mNormals[i]),
            sizeof(DirectX::XMFLOAT3),
            buffer.begin() + pos
        );
        pos += sizeof(DirectX::XMFLOAT3);

        std::copy_n(
            reinterpret_cast<uint8_t*>(&ai_mesh->mTextureCoords[0][i]),
            sizeof(DirectX::XMFLOAT2),
            buffer.begin() + pos
        );
        pos += sizeof(DirectX::XMFLOAT2);
    }
}

void Engine::Graphics::D3D11VertexAttribute::ParseVertex_1P1N1T(aiMesh const*& ai_mesh, x_vector<uint8_t>& buffer) {
    size_t pos {0u};
    for (auto i {size_t{}}; i != ai_mesh->mNumVertices; ++i) {
        std::copy_n(
            reinterpret_cast<uint8_t*>(&ai_mesh->mVertices[i]),
            sizeof(DirectX::XMFLOAT3),
            buffer.begin() + pos
        );
        pos += sizeof(DirectX::XMFLOAT3);

        std::copy_n(
            reinterpret_cast<uint8_t*>(&ai_mesh->mNormals[i]),
            sizeof(DirectX::XMFLOAT3),
            buffer.begin() + pos
        );
        pos += sizeof(DirectX::XMFLOAT3);

        std::copy_n(
            reinterpret_cast<uint8_t*>(&ai_mesh->mTangents[i]),
            sizeof(DirectX::XMFLOAT3),
            buffer.begin() + pos
        );
        pos += sizeof(DirectX::XMFLOAT3);

        std::copy_n(
            reinterpret_cast<uint8_t*>(&ai_mesh->mBitangents[i]),
            sizeof(DirectX::XMFLOAT3),
            buffer.begin() + pos
        );
        pos += sizeof(DirectX::XMFLOAT3);
    }
}

void Engine::Graphics::D3D11VertexAttribute::ParseVertex_1P1N1T1UV(aiMesh const*& ai_mesh, x_vector<uint8_t>& buffer) {
    size_t pos {0u};
    for (auto i {size_t{}}; i != ai_mesh->mNumVertices; ++i) {
        std::copy_n(
            reinterpret_cast<uint8_t*>(&ai_mesh->mVertices[i]),
            sizeof(DirectX::XMFLOAT3),
            buffer.begin() + pos
        );
        pos += sizeof(DirectX::XMFLOAT3);

        std::copy_n(
            reinterpret_cast<uint8_t*>(&ai_mesh->mNormals[i]),
            sizeof(DirectX::XMFLOAT3),
            buffer.begin() + pos
        );
        pos += sizeof(DirectX::XMFLOAT3);

        std::copy_n(
            reinterpret_cast<uint8_t*>(&ai_mesh->mTangents[i]),
            sizeof(DirectX::XMFLOAT3),
            buffer.begin() + pos
        );
        pos += sizeof(DirectX::XMFLOAT3);

        std::copy_n(
            reinterpret_cast<uint8_t*>(&ai_mesh->mBitangents[i]),
            sizeof(DirectX::XMFLOAT3),
            buffer.begin() + pos
        );
        pos += sizeof(DirectX::XMFLOAT3);

        std::copy_n(
            reinterpret_cast<uint8_t*>(&ai_mesh->mTextureCoords[0][i]),
            sizeof(DirectX::XMFLOAT2),
            buffer.begin() + pos
        );
        pos += sizeof(DirectX::XMFLOAT2);
    }
}
