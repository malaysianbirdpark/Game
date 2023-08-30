#include "pch.h"
#include "D3D11PSOLibrary.h"

#include "D3D11PipelineStateObject.h"
#include "D3D11VertexShader.h"
#include "Graphics/D3D11/SceneGraph/D3D11VertexAttribute.h"
#include "Graphics/D3D11/PipelineState/D3D11InputLayout.h"

void Engine::Graphics::D3D11PSOLibrary::Init(ID3D11Device& device) {
    InitSolid(device);
    InitPhong(device);
    InitPBR(device);
    InitCubemap(device);
    InitFilters(device);
    InitEnvironmentMapping(device);
    //InitBasicIBL(device);
}

void Engine::Graphics::D3D11PSOLibrary::RegisterPSO(x_string const& tag, std::shared_ptr<D3D11PipelineStateObject> const& pso) {
    if (_lib.contains(tag))
        return;

    _lib[tag] = std::move(pso);
}

std::shared_ptr<Engine::Graphics::D3D11PipelineStateObject> Engine::Graphics::D3D11PSOLibrary::ResolvePSO(x_string const& tag) {
    CORE_ASSERT(_lib.contains(tag), "Failed to resolve PSO")

    return _lib[tag];
}

void Engine::Graphics::D3D11PSOLibrary::InitSolid(ID3D11Device& device) {
    auto const pso {MakeShared<D3D11PipelineStateObject>()};

    pso->SetVertexShader(device, "./ShaderLib/solid_VS.cso");
    pso->SetInputLayout(device, D3D11VertexAttribute::_1p1n1t1uvLayout);
    pso->SetPixelShader(device, "./ShaderLib/solid_PS.cso");

    x_string const _tag {"solid"};
    RegisterPSO(_tag, pso);
}

void Engine::Graphics::D3D11PSOLibrary::InitPhong(ID3D11Device& device) {
    {
        auto const pso {MakeShared<D3D11PipelineStateObject>()};

        pso->SetVertexShader(device, "./ShaderLib/phong_VS.cso");
        pso->SetInputLayout(device, D3D11VertexAttribute::_1p1n1t1uvLayout);
        pso->SetPixelShader(device, "./ShaderLib/phong_PS.cso");

        x_string const _tag {"phong"};
        RegisterPSO(_tag, pso);
    }
}

void Engine::Graphics::D3D11PSOLibrary::InitPBR(ID3D11Device& device) {
    {
        auto const pso {MakeShared<D3D11PipelineStateObject>()};

        pso->SetVertexShader(device, "./ShaderLib/unreal_pbr_VS.cso");
        pso->SetInputLayout(device, D3D11VertexAttribute::_1p1n1t1uvLayout);
        pso->SetPixelShader(device, "./ShaderLib/unreal_pbr_PS.cso");

        x_string const _tag {"unreal_pbr"};
        RegisterPSO(_tag, pso);
    }
}

void Engine::Graphics::D3D11PSOLibrary::InitCubemap(ID3D11Device& device) {
    auto const pso {MakeShared<D3D11PipelineStateObject>()};

    pso->SetVertexShader(device, "./ShaderLib/pos_VS.cso");
    pso->SetInputLayout(device, D3D11VertexAttribute::_1p1n1t1uvLayout);
    pso->SetPixelShader(device, "./ShaderLib/cubemap_PS.cso");

    x_string const _tag {"cubemap"};
    RegisterPSO(_tag, pso);
}

void Engine::Graphics::D3D11PSOLibrary::InitFilters(ID3D11Device& device) {
    // Fullscreen Blur
    {
        auto const pso {MakeShared<D3D11PipelineStateObject>()};

        UINT aligned_byte_offset {0u};
        x_vector<D3D11_INPUT_ELEMENT_DESC> layout;
        layout.push_back(
            {"POSITION", 0u, DXGI_FORMAT_R32G32_FLOAT, 0u, aligned_byte_offset, D3D11_INPUT_PER_VERTEX_DATA, 0u}
        );
        aligned_byte_offset += sizeof(DirectX::XMFLOAT2);

        pso->SetVertexShader(device, "./ShaderLib/fullscreen_filter_Vs.cso");
        pso->SetInputLayout(device, layout);
        pso->SetPixelShader(device, "./ShaderLib/fullscreen_blur_PS.cso");

        x_string const _tag {"fullscreen_blur"};
        RegisterPSO(_tag, pso);
    }
}

void Engine::Graphics::D3D11PSOLibrary::InitEnvironmentMapping(ID3D11Device& device) {
    // normal
    {
        auto const pso {MakeShared<D3D11PipelineStateObject>()};

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

        pso->SetVertexShader(device, "./ShaderLib/environmapping_normal_VS.cso");
        pso->SetInputLayout(device, D3D11VertexAttribute::_1p1n1t1uvLayout);
        pso->SetPixelShader(device, "./ShaderLib/environmapping_normal_PS.cso");

        x_string const _tag {"environment_mapping_normal"};
        RegisterPSO(_tag, pso);
    }

    // reflect
    {
        auto const pso {MakeShared<D3D11PipelineStateObject>()};

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

        pso->SetVertexShader(device, "./ShaderLib/environmapping_reflect_VS.cso");
        pso->SetInputLayout(device, D3D11VertexAttribute::_1p1n1t1uvLayout);
        pso->SetPixelShader(device, "./ShaderLib/environmapping_reflect_PS.cso");

        x_string const _tag {"environment_mapping_reflect"};
        RegisterPSO(_tag, pso);
    }
}

void Engine::Graphics::D3D11PSOLibrary::InitBasicIBL(ID3D11Device& device) {
    // basic
    {
        auto const pso {MakeShared<D3D11PipelineStateObject>()};

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

        pso->SetVertexShader(device, "./ShaderLib/basic_ibl_VS.cso");
        pso->SetInputLayout(device, layout);
        pso->SetPixelShader(device, "./ShaderLib/basic_ibl_PS.cso");

        x_string const _tag {"basic_ibl"};
        RegisterPSO(_tag, pso);
    }

    // textured
    {
        auto const pso {MakeShared<D3D11PipelineStateObject>()};

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

        pso->SetVertexShader(device, "./ShaderLib/basic_ibl_tex_VS.cso");
        pso->SetInputLayout(device, layout);
        pso->SetPixelShader(device, "./ShaderLib/basic_ibl_tex_PS.cso");

        x_string const _tag {"basic_ibl_textured"};
        RegisterPSO(_tag, pso);
    }

}
