#include "pch.h"
#include "D3D11PhongMaterialConfiguration.h"

Engine::Graphics::D3D11PhongMaterialConfiguration::D3D11PhongMaterialConfiguration(ID3D11Device& device, DirectX::XMFLOAT3&& dir, DirectX::XMFLOAT3&& color)
    : _info{._diffuseColor = std::move(color), ._dir = std::move(dir)}
{
    D3D11_BUFFER_DESC bd {};
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bd.MiscFlags = 0;
    bd.ByteWidth = sizeof(MaterialParameters); 
    bd.StructureByteStride = 0u;

    D3D11_SUBRESOURCE_DATA sd {};
    sd.pSysMem = &_info;

    device.CreateBuffer(&bd, &sd, _resource.ReleaseAndGetAddressOf());
}

void Engine::Graphics::D3D11PhongMaterialConfiguration::Update() {
    ImGuiShowEditWindow();
}

void Engine::Graphics::D3D11PhongMaterialConfiguration::Upload(ID3D11DeviceContext& context) {
    D3D11_MAPPED_SUBRESOURCE msr {};
    context.Map(
        _resource.Get(),
        0u,
        D3D11_MAP_WRITE_DISCARD,
        0u,
        &msr
    );
    memcpy(msr.pData, &_info, sizeof(MaterialParameters));
    context.Unmap(_resource.Get(), 0u);
}

void Engine::Graphics::D3D11PhongMaterialConfiguration::Bind(ID3D11DeviceContext& context) {
    Upload(context);
    context.PSSetConstantBuffers(0u, 1u, _resource.GetAddressOf());
}

