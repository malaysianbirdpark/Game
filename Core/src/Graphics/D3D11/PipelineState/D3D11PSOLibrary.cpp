#include "pch.h"
#include "D3D11PSOLibrary.h"

#include "D3D11PipelineStateObject.h"

void Engine::Graphics::D3D11PSOLibrary::Init(ID3D11Device& device) {
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
        RegisterPSO(_tag, pso);
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

        x_string const _tag {"textured"};
        RegisterPSO(_tag, pso);
    }

    // phong-tex
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

        pso->SetVertexShader(device, "./ShaderLib/phong_tex_VS.cso");
        pso->SetInputLayout(device, layout);
        pso->SetPixelShader(device, "./ShaderLib/phong_tex_PS.cso");

        x_string const _tag {"phong_tex"};
        RegisterPSO(_tag, pso);
    }
}

void Engine::Graphics::D3D11PSOLibrary::RegisterPSO(x_string const& tag, std::shared_ptr<D3D11PipelineStateObject> const& pso) {
    if (_lib.contains(tag))
        return;

    _lib[tag] = std::move(pso);
}

std::shared_ptr<Engine::Graphics::D3D11PipelineStateObject> Engine::Graphics::D3D11PSOLibrary::ResolvePSO(x_string const& tag) {
    if (!_lib.contains(tag))
        return nullptr;

    return _lib[tag];
}
