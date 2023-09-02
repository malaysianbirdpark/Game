#include "pch.h"
#include "D3D11SolidConstant.h"

Engine::Graphics::D3D11SolidConstants::D3D11SolidConstants(ID3D11Device& device) {
    D3D11_BUFFER_DESC bd {};
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bd.MiscFlags = 0;
    bd.ByteWidth = sizeof(SolidParameters); 
    bd.StructureByteStride = 0u;

    D3D11_SUBRESOURCE_DATA sd {};
    sd.pSysMem = &_params;

    device.CreateBuffer(&bd, &sd, _resource.ReleaseAndGetAddressOf());
}

Engine::Graphics::D3D11SolidConstants::SolidParameters& Engine::Graphics::D3D11SolidConstants::GetParams() {
    return _params;
}

void Engine::Graphics::D3D11SolidConstants::Upload(ID3D11DeviceContext& context) {
    D3D11_MAPPED_SUBRESOURCE msr {};
    context.Map(
        _resource.Get(),
        0u,
        D3D11_MAP_WRITE_DISCARD,
        0u,
        &msr
    );
    memcpy(msr.pData, &_params, sizeof(SolidParameters));
    context.Unmap(_resource.Get(), 0u);
}

void Engine::Graphics::D3D11SolidConstants::Bind(ID3D11DeviceContext& context) {
    Upload(context);
    context.VSSetConstantBuffers(1u, 1u, _resource.GetAddressOf());
    context.PSSetConstantBuffers(1u, 1u, _resource.GetAddressOf());
}

