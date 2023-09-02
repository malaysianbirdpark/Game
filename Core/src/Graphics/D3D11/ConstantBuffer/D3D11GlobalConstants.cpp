#include "pch.h"
#include "D3D11GlobalConstants.h"

#include "Graphics/D3DCamera.h"
#include "Graphics/D3D11/D3D11Core.h"

Engine::Graphics::D3D11GlobalConstants::D3D11GlobalConstants(ID3D11Device& device) {
    DirectX::XMStoreFloat4(&_data._camPos, D3DCamera::GetPos());

    D3D11_BUFFER_DESC bd {};
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bd.MiscFlags = 0;
    bd.ByteWidth = sizeof(Constants); 
    bd.StructureByteStride = 0u;

    D3D11_SUBRESOURCE_DATA sd {};
    sd.pSysMem = &_data;

    device.CreateBuffer(&bd, &sd, _resource.ReleaseAndGetAddressOf());
}

void Engine::Graphics::D3D11GlobalConstants::Reflect(DirectX::XMVECTOR plane) {
    //using namespace DirectX;
    //XMStoreFloat4x4(
    //    &_data._vp,
    //    XMMatrixTranspose(
    //        XMMatrixMultiply(
    //            XMLoadFloat4x4(&_data._vp),
    //            XMMatrixReflect(plane)
    //        )
    //    )
    //);
}

void Engine::Graphics::D3D11GlobalConstants::SetLightType(LightType type) {
    _data._light_type = static_cast<int32_t>(type);
}

void Engine::Graphics::D3D11GlobalConstants::Update(float const dt) {
    DirectX::XMStoreFloat4(&_data._camPos, D3DCamera::GetPos());
}

void Engine::Graphics::D3D11GlobalConstants::Upload(ID3D11DeviceContext& context) {
    D3D11_MAPPED_SUBRESOURCE msr {};
    context.Map(
        _resource.Get(),
        0u,
        D3D11_MAP_WRITE_DISCARD,
        0u,
        &msr
    );
    memcpy(msr.pData, &_data, sizeof(Constants));
    context.Unmap(_resource.Get(), 0u);
}

void Engine::Graphics::D3D11GlobalConstants::Bind(ID3D11DeviceContext& context) {
    Upload(context);
    context.PSSetConstantBuffers(2u, 1u, _resource.GetAddressOf());
}
