#include "pch.h"
#include "D3D11CamPosition.h"

#include "Graphics/D3DCamera.h"

Engine::Graphics::D3D11CamPosition::D3D11CamPosition(ID3D11Device& device, DirectX::XMVECTOR const& pos) {
    DirectX::XMStoreFloat4(&_pos, pos);

    D3D11_BUFFER_DESC bd {};
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bd.MiscFlags = 0;
    bd.ByteWidth = sizeof(DirectX::XMFLOAT4); 
    bd.StructureByteStride = 0u;

    D3D11_SUBRESOURCE_DATA sd {};
    sd.pSysMem = &_pos;

    device.CreateBuffer(&bd, &sd, _resource.ReleaseAndGetAddressOf());
}

void Engine::Graphics::D3D11CamPosition::Update() {
    DirectX::XMStoreFloat4(&_pos, D3DCamera::GetPos());
}

void Engine::Graphics::D3D11CamPosition::Upload(ID3D11DeviceContext& context) {
    D3D11_MAPPED_SUBRESOURCE msr {};
    context.Map(
        _resource.Get(),
        0u,
        D3D11_MAP_WRITE_DISCARD,
        0u,
        &msr
    );
    memcpy(msr.pData, &_pos, sizeof(DirectX::XMFLOAT4));
    context.Unmap(_resource.Get(), 0u);
}

void Engine::Graphics::D3D11CamPosition::Bind(ID3D11DeviceContext& context) {
    Upload(context);
    context.PSSetConstantBuffers(0u, 1u, _resource.GetAddressOf());
}
