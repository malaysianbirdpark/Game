#include "pch.h"
#include "D3D11UnrealPBRConstants.h"

Engine::Graphics::D3D11UnrealPBRConstants::D3D11UnrealPBRConstants(ID3D11Device& device) {
    D3D11_BUFFER_DESC bd {};
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bd.MiscFlags = 0;
    bd.ByteWidth = sizeof(UnrealPBRParameters); 
    bd.StructureByteStride = 0u;

    D3D11_SUBRESOURCE_DATA sd {};
    sd.pSysMem = &_params;

    device.CreateBuffer(&bd, &sd, _resource.ReleaseAndGetAddressOf());
}

Engine::Graphics::D3D11UnrealPBRConstants::UnrealPBRParameters& Engine::Graphics::D3D11UnrealPBRConstants::GetParams() {
    return _params;
}

void Engine::Graphics::D3D11UnrealPBRConstants::Upload(ID3D11DeviceContext& context) {
    D3D11_MAPPED_SUBRESOURCE msr {};
    context.Map(
        _resource.Get(),
        0u,
        D3D11_MAP_WRITE_DISCARD,
        0u,
        &msr
    );
    memcpy(msr.pData, &_params, sizeof(UnrealPBRParameters));
    context.Unmap(_resource.Get(), 0u);
}

void Engine::Graphics::D3D11UnrealPBRConstants::Bind(ID3D11DeviceContext& context) {
    Upload(context);
    context.VSSetConstantBuffers(1u, 1u, _resource.GetAddressOf());
    context.PSSetConstantBuffers(1u, 1u, _resource.GetAddressOf());
}

