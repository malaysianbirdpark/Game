#include "pch.h"
#include "D3D11ModelQuery.h"

#include "D3D11MeshDataHolder.h"
#include "Graphics/D3D11/PipelineState/D3D11PipelineStateObject.h"
#include "Graphics/D3D11/PipelineState/D3D11PSOLibrary.h"

#include <assimp/material.h>

#include <filesystem>

void Engine::Graphics::D3D11ModelQuery::Init(ID3D11Device& device, DirectX::XMMATRIX const& proj) {
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
    }

    // solid diffuse-only texture
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

        pso->SetVertexShader(device, "./ShaderLib/solid_pos3_nor_tex2_dif_VS.cso");
        pso->SetInputLayout(device, layout);
        pso->SetPixelShader(device, "./ShaderLib/solid_pos3_nor_tex2_dif_PS.cso");

        x_string const _tag {"solid_pos3_nor_tex2_dif"};
        D3D11PSOLibrary::RegisterPSO(_tag, pso);
    }
}

void Engine::Graphics::D3D11ModelQuery::QueryMaterial(aiMaterial const* ai_material, x_string& tag) {
    if (auto const count {ai_material->GetTextureCount(aiTextureType_DIFFUSE)}; count >= 1) {
        tag += "_dif";

        aiString ai_path {};
        for (auto i {0u}; i != count; ++i) {
            ai_material->GetTexture(aiTextureType_DIFFUSE, i, &ai_path);

            auto path {std::filesystem::path{ai_path.C_Str()}};
            if (path.extension() != ".dds")
                path.replace_extension(".dds");
            std::cout << path << std::endl;
        }
    }
}

std::tuple<Engine::x_string, Engine::Graphics::D3D11ModelQuery::vb, Engine::Graphics::D3D11ModelQuery::ib> Engine::Graphics::D3D11ModelQuery::Cook(ID3D11Device& device, aiMesh const* ai_mesh, aiMaterial const* ai_material) {
    x_string tag {"solid"};

    QueryVertexLayout(ai_mesh);
    CookVertexLayout(tag);
    auto [vertex_buffer, index_buffer] {CookVertexBuffer(device, ai_mesh)};

    QueryMaterial(ai_material, tag);

    return {tag, vertex_buffer, index_buffer}; 
}

void Engine::Graphics::D3D11ModelQuery::CookVertexLayout(x_string& tag) {
    if (_vertexFlag & Position3D) {
        _layout.push_back(
            {"POSITION", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, _stride, D3D11_INPUT_PER_VERTEX_DATA, 0u}
        );
        _stride += sizeof(DirectX::XMFLOAT3);
        tag += "_pos3";
    }

    if (_vertexFlag & Normal) {
        _layout.push_back(
            {"Normal", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, _stride, D3D11_INPUT_PER_VERTEX_DATA, 0u}
        );
        _stride += sizeof(DirectX::XMFLOAT3);
        tag += "_nor";
    }

    if (_vertexFlag & Tangent) {
        _layout.push_back(
            {"TANGENT", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, _stride, D3D11_INPUT_PER_VERTEX_DATA, 0u}
        );
        _stride += sizeof(DirectX::XMFLOAT3);
        tag += "_tan";
    }

    if (_vertexFlag & BiTangent) {
        _layout.push_back(
            {"BINORMAL", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, _stride, D3D11_INPUT_PER_VERTEX_DATA, 0u}
        );
        _stride += sizeof(DirectX::XMFLOAT3);
        tag += "_bit";
    }

    if (_vertexFlag & Texture2D) {
        _layout.push_back(
            {"TEXCOORD", 0u, DXGI_FORMAT_R32G32_FLOAT, 0u, _stride, D3D11_INPUT_PER_VERTEX_DATA, 0u}
        );
        _stride += sizeof(DirectX::XMFLOAT2);
        tag += "_tex2";
    }
}

Engine::x_vector<int8_t> Engine::Graphics::D3D11ModelQuery::CookVertex(aiMesh const* ai_mesh, size_t idx) {
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

std::pair<Engine::Graphics::D3D11ModelQuery::vb, Engine::Graphics::D3D11ModelQuery::ib> Engine::Graphics::D3D11ModelQuery::CookVertexBuffer(ID3D11Device& device, aiMesh const* ai_mesh) {
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
