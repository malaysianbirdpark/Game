#include "pch.h"
#include "D3DRenderTech.h"

#include "D3D11/D3D11MeshDataHolder.h"
#include "D3D11/PipelineState/D3D11PipelineStateObject.h"
#include "D3D11/PipelineState/D3D11PSOLibrary.h"

#include "D3D11/RootSignature/D3D11RootSignature.h"
#include "D3D11/RootSignature/D3D11RSLibrary.h"

#include <algorithm>

void Engine::Graphics::D3DRenderTech::Init(ID3D11Device& device, DirectX::XMMATRIX const& proj) {
    if (_initiated)
        return;

    _initiated = true;

    // solid
    {
        auto pso {MakeShared<D3D11PipelineStateObject>()};

        UINT aligned_byte_offset {0u};
        x_vector<D3D11_INPUT_ELEMENT_DESC> layout;
        layout.push_back(
            {"POSITION", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, aligned_byte_offset, D3D11_INPUT_PER_VERTEX_DATA, 0u}
        );
        aligned_byte_offset += sizeof(DirectX::XMFLOAT3);
        layout.push_back(
            {"NORMAL", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, aligned_byte_offset, D3D11_INPUT_PER_VERTEX_DATA, 0u}
        );
        aligned_byte_offset += sizeof(DirectX::XMFLOAT3);

        pso->SetVertexShader(device, "./ShaderLib/solid_VS.cso");
        pso->SetInputLayout(device, layout);
        pso->SetPixelShader(device, "./ShaderLib/solid_PS.cso");

        x_string const _tag {"solid"};
        D3D11PSOLibrary::RegisterPSO(_tag, pso);

        auto const rs {MakeShared<D3D11RootSignature>(device, proj, pso, 100)};

        D3D11RSLibrary::RegisterRS(_tag, rs);
    }

    // solid texture
    {
        auto pso {MakeShared<D3D11PipelineStateObject>()};

        UINT aligned_byte_offset {0u};
        x_vector<D3D11_INPUT_ELEMENT_DESC> layout;
        layout.push_back(
            {"POSITION", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, aligned_byte_offset, D3D11_INPUT_PER_VERTEX_DATA, 0u}
        );
        aligned_byte_offset += sizeof(DirectX::XMFLOAT3);
        layout.push_back(
            {"NORMAL", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, aligned_byte_offset, D3D11_INPUT_PER_VERTEX_DATA, 0u}
        );
        aligned_byte_offset += sizeof(DirectX::XMFLOAT3);
        layout.push_back(
            {"TEXCOORD", 0u, DXGI_FORMAT_R32G32_FLOAT, 0u, aligned_byte_offset, D3D11_INPUT_PER_VERTEX_DATA, 0u}
        );
        aligned_byte_offset += sizeof(DirectX::XMFLOAT2);


        pso->SetVertexShader(device, "./ShaderLib/solid_texture_VS.cso");
        pso->SetInputLayout(device, layout);
        pso->SetPixelShader(device, "./ShaderLib/solid_texture_PS.cso");

        x_string const _tag {"solid_texture"};
        D3D11PSOLibrary::RegisterPSO(_tag, pso);

        auto const rs {MakeShared<D3D11RootSignature>(device, proj, pso, 100)};

        D3D11RSLibrary::RegisterRS(_tag, rs);
    }
}

void Engine::Graphics::D3DRenderTech::QueryVertexLayout(aiMesh const* ai_mesh) {
    _vertexFlag |= ai_mesh->HasPositions() *              Position3D;
    _vertexFlag |= ai_mesh->HasNormals() *                Normal;
    _vertexFlag |= ai_mesh->HasTangentsAndBitangents() *  Tangent;
    _vertexFlag |= ai_mesh->HasTangentsAndBitangents() *  BiTangent;
    _vertexFlag |= ai_mesh->HasTextureCoords(0) *   Texture2D;
}

void Engine::Graphics::D3DRenderTech::QueryMaterialProp(aiMesh const* ai_mesh, aiMaterial const* ai_material) {
}

std::pair<Engine::Graphics::D3DRenderTech::vb, Engine::Graphics::D3DRenderTech::ib> Engine::Graphics::D3DRenderTech::Cook(ID3D11Device& device, aiMesh const* ai_mesh, aiMaterial const* ai_material) {
    return CookVertexBuffer(device, ai_mesh);
}

void Engine::Graphics::D3DRenderTech::CookVertexLayout() {
    if (_vertexFlag & Position3D) {
        _layout.push_back(
            {"POSITION", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, _stride, D3D11_INPUT_PER_VERTEX_DATA, 0u}
        );
        _stride += sizeof(DirectX::XMFLOAT3);
    }

    if (_vertexFlag & Normal) {
        _layout.push_back(
            {"Normal", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, _stride, D3D11_INPUT_PER_VERTEX_DATA, 0u}
        );
        _stride += sizeof(DirectX::XMFLOAT3);
    }

    if (_vertexFlag & Tangent) {
        _layout.push_back(
            {"TANGENT", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, _stride, D3D11_INPUT_PER_VERTEX_DATA, 0u}
        );
        _stride += sizeof(DirectX::XMFLOAT3);
    }

    if (_vertexFlag & BiTangent) {
        _layout.push_back(
            {"BINORMAL", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, _stride, D3D11_INPUT_PER_VERTEX_DATA, 0u}
        );
        _stride += sizeof(DirectX::XMFLOAT3);
    }

    if (_vertexFlag & Texture2D) {
        _layout.push_back(
            {"TEXCOORD", 0u, DXGI_FORMAT_R32G32_FLOAT, 0u, _stride, D3D11_INPUT_PER_VERTEX_DATA, 0u}
        );
        _stride += sizeof(DirectX::XMFLOAT2);
    }
}

Engine::x_vector<int8_t> Engine::Graphics::D3DRenderTech::CookVertex(aiMesh const* ai_mesh, size_t idx) {
    x_vector<int8_t> vertex (_stride, 0);

    size_t pos {};
    if (_vertexFlag & Position3D) {
        auto const data {reinterpret_cast<int8_t*>(&ai_mesh->mVertices[idx])};
        std::copy_n(data, sizeof(DirectX::XMFLOAT3), vertex.begin() + pos);
        pos += sizeof(DirectX::XMFLOAT3);
    }

    if (_vertexFlag & Normal) {
        auto const data {reinterpret_cast<int8_t*>(&ai_mesh->mNormals[idx])};
        std::copy_n(data, sizeof(DirectX::XMFLOAT3), vertex.begin() + pos);
        pos += sizeof(DirectX::XMFLOAT3);
    }

    if (_vertexFlag & Tangent) {
        auto const data {reinterpret_cast<int8_t*>(&ai_mesh->mTangents[idx])};
        std::copy_n(data, sizeof(DirectX::XMFLOAT3), vertex.begin() + pos);
        pos += sizeof(DirectX::XMFLOAT3);
    }

    if (_vertexFlag & BiTangent) {
        auto const data {reinterpret_cast<int8_t*>(&ai_mesh->mBitangents[idx])};
        std::copy_n(data, sizeof(DirectX::XMFLOAT3), vertex.begin() + pos);
        pos += sizeof(DirectX::XMFLOAT3);
    }

    if (_vertexFlag & Texture2D) {
        auto const data {reinterpret_cast<int8_t*>(&ai_mesh->mTextureCoords[0][idx])};
        std::copy_n(data, sizeof(DirectX::XMFLOAT2), vertex.begin() + pos);
        pos += sizeof(DirectX::XMFLOAT2);
    }

    return vertex;
}

std::pair<Engine::Graphics::D3DRenderTech::vb, Engine::Graphics::D3DRenderTech::ib> Engine::Graphics::D3DRenderTech::CookVertexBuffer(ID3D11Device& device, aiMesh const* ai_mesh) {
    CookVertexLayout();

    _vertices.resize(_stride * ai_mesh->mNumVertices);
    for (auto i {0}; i != ai_mesh->mNumVertices; ++i) {
        auto const vertex {CookVertex(ai_mesh, i)};
        std::copy_n(vertex.begin(), _stride, _vertices.begin() + _stride * i);
    }

    x_vector<uint16_t> indices;
    indices.reserve(ai_mesh->mNumFaces * 3);
    for (auto i {0}; i != ai_mesh->mNumFaces; ++i) {
        auto const& face {ai_mesh->mFaces[i]};
        indices.push_back(face.mIndices[0]);
        indices.push_back(face.mIndices[1]);
        indices.push_back(face.mIndices[2]);
    }

    auto vertex_buffer {D3D11MeshDataHolder::ResolveVertexBuffer(device, _stride, _vertices.size(), _vertices.data(), ai_mesh->mName.C_Str())};
    auto index_buffer {D3D11MeshDataHolder::ResolveIndexBuffer(device, indices, ai_mesh->mName.C_Str())};

    return {vertex_buffer, index_buffer};
}
