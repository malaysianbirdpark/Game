#include "pch.h"
#include "D3D11TransformCB.h"

#include "Graphics/D3DCamera.h"
#include "Graphics/D3D11/D3D11Core.h"

Engine::Graphics::D3D11TransformCB::D3D11TransformCB(ID3D11Device& device, ID3D11DeviceContext& context, DirectX::CXMMATRIX model) {
    D3D11_BUFFER_DESC bd {};
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bd.MiscFlags = 0;
    bd.ByteWidth = sizeof(Transform); 
    bd.StructureByteStride = 0u;

    using namespace DirectX;

    XMStoreFloat4x4(&_model, model);

    D3D11_SUBRESOURCE_DATA sd {};
    XMStoreFloat4x4(&_transform._m, XMMatrixTranspose(model));
    XMStoreFloat4x4(&_transform._mit, XMMatrixInverse(nullptr, model));
    XMStoreFloat4x4(&_transform._mvp, XMMatrixTranspose(XMMatrixMultiply(XMMatrixMultiply(model, D3DCamera::GetView()), D3D11Core::GetProj())));
    sd.pSysMem = &_transform;

    device.CreateBuffer(&bd, &sd, _resource.ReleaseAndGetAddressOf());
    Update(context);
}

void Engine::Graphics::D3D11TransformCB::Accumulate(DirectX::CXMMATRIX transform) {
    DirectX::XMStoreFloat4x4(&_model, DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&_model), transform));
}

DirectX::XMMATRIX Engine::Graphics::D3D11TransformCB::GetModel() const {
    return DirectX::XMLoadFloat4x4(&_model);
}

void Engine::Graphics::D3D11TransformCB::SetModel(DirectX::CXMMATRIX model) {
    DirectX::XMStoreFloat4x4(&_model, model);
}

void Engine::Graphics::D3D11TransformCB::Update(ID3D11DeviceContext& context) {
    using namespace DirectX;
    auto const model {XMLoadFloat4x4(&_model)};
    XMStoreFloat4x4(&_transform._m, XMMatrixTranspose(model));
    XMStoreFloat4x4(&_transform._mit, XMMatrixInverse(nullptr, model));
    XMStoreFloat4x4(&_transform._mvp, XMMatrixTranspose(XMMatrixMultiply(XMMatrixMultiply(model, D3DCamera::GetView()), D3D11Core::GetProj())));

    D3D11_MAPPED_SUBRESOURCE msr {};
    context.Map(
        _resource.Get(),
        0u,
        D3D11_MAP_WRITE_DISCARD,
        0u,
        &msr
    );
    memcpy(msr.pData, &_transform, sizeof(Transform));
    context.Unmap(_resource.Get(), 0u);
}

void Engine::Graphics::D3D11TransformCB::Bind(ID3D11DeviceContext& context) const {
    context.VSSetConstantBuffers(0u, 1u, _resource.GetAddressOf());
}
