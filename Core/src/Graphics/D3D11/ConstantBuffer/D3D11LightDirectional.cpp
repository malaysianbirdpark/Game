#include "pch.h"
#include "D3D11LightDirectional.h"
#include "D3D11LightDirectional.h"

Engine::Graphics::D3D11LightDirectional::D3D11LightDirectional(ID3D11Device& device, DirectX::XMFLOAT3 const& dir, DirectX::XMFLOAT4 const& color, float att)
    : _info{dir, color, att}
{
    D3D11_BUFFER_DESC bd {};
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bd.MiscFlags = 0;
    bd.ByteWidth = sizeof(LightInfo); 
    bd.StructureByteStride = 0u;

    D3D11_SUBRESOURCE_DATA sd {};
    sd.pSysMem = &_info;

    device.CreateBuffer(&bd, &sd, _resource.ReleaseAndGetAddressOf());
}

void Engine::Graphics::D3D11LightDirectional::UpdateDir(ID3D11DeviceContext& context, DirectX::XMFLOAT3 const& dir) {
    _info._dir = dir;
    Update(context);
}

void Engine::Graphics::D3D11LightDirectional::UpdateColor(ID3D11DeviceContext& context, DirectX::XMFLOAT4 const& color) {
    _info._color = color;
    Update(context);
}

void Engine::Graphics::D3D11LightDirectional::UpdateAttenuation(ID3D11DeviceContext& context, float const att) {
    _info._att = att;
    Update(context);
}

void Engine::Graphics::D3D11LightDirectional::Update(ID3D11DeviceContext& context) {
    D3D11_MAPPED_SUBRESOURCE msr {};
    context.Map(
        _resource.Get(),
        0u,
        D3D11_MAP_WRITE_DISCARD,
        0u,
        &msr
    );
    memcpy(msr.pData, &_info, sizeof(LightInfo));
    context.Unmap(_resource.Get(), 0u);
}

void Engine::Graphics::D3D11LightDirectional::Bind(ID3D11DeviceContext& context) const {
    context.PSSetConstantBuffers(1u, 1u, _resource.GetAddressOf());
}
