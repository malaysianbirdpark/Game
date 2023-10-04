#include "pch.h"
#include "D3D11PipelineStateObject.h"

#include <d3dcompiler.h>

#include "D3D11PipelineStateHolder.h"

void Engine::Graphics::D3D11PipelineStateObject::SetInputLayout(ID3D11Device& device, x_vector<D3D11_INPUT_ELEMENT_DESC> const& layout) {
    CORE_ASSERT(_vs, "Set VertexShader First!");

    device.CreateInputLayout(
        layout.data(),
        layout.size(),
        _vsBytesCode->GetBufferPointer(),
        _vsBytesCode->GetBufferSize(),
        _inputLayout.ReleaseAndGetAddressOf()
    );
}

void Engine::Graphics::D3D11PipelineStateObject::SetPixelShader(ID3D11Device& device, char const* path) {
    Microsoft::WRL::ComPtr<ID3DBlob> blob;

    std::string const _path {path};
    std::wstring p (_path.length(), L' ');
    std::ranges::copy(_path, p.begin());

    D3DReadFileToBlob(p.c_str(), blob.ReleaseAndGetAddressOf());
    device.CreatePixelShader(
        blob->GetBufferPointer(), 
        blob->GetBufferSize(), 
        nullptr, 
        _ps.ReleaseAndGetAddressOf()
    );
}

void Engine::Graphics::D3D11PipelineStateObject::SetVertexShader(ID3D11Device& device, char const* path) {
    std::string const _path {path};
    std::wstring p (_path.length(), L' ');
    std::ranges::copy(_path, p.begin());

    D3DReadFileToBlob(
        p.c_str(),
        _vsBytesCode.ReleaseAndGetAddressOf()
    );

    device.CreateVertexShader(
        _vsBytesCode->GetBufferPointer(),
        _vsBytesCode->GetBufferSize(),
        nullptr,
        _vs.ReleaseAndGetAddressOf()
    );
}

void Engine::Graphics::D3D11PipelineStateObject::SetGeometryShader(ID3D11Device& device, char const* path) {
    Microsoft::WRL::ComPtr<ID3DBlob> blob;

    std::string const _path {path};
    std::wstring p (_path.length(), L' ');
    std::ranges::copy(_path, p.begin());

    D3DReadFileToBlob(p.c_str(), blob.ReleaseAndGetAddressOf());
    device.CreateGeometryShader(
        blob->GetBufferPointer(), 
        blob->GetBufferSize(), 
        nullptr, 
        _gs.ReleaseAndGetAddressOf()
    );
}

void Engine::Graphics::D3D11PipelineStateObject::Bind(ID3D11DeviceContext& context) {
    context.IASetInputLayout(_inputLayout.Get());
    context.VSSetShader(_vs.Get(), nullptr, 0u);
    context.PSSetShader(_ps.Get(), nullptr, 0u);
    context.GSSetShader(_gs.Get(), nullptr, 0u);
}

