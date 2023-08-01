#include "pch.h"
#include "D3D11TransformMVP.h"

Engine::Graphics::D3D11TransformMVP::D3D11TransformMVP(ID3D11Device& device, DirectX::XMMATRIX const& model, DirectX::XMMATRIX const& view, DirectX::XMMATRIX const& proj) {
    D3D11_BUFFER_DESC bd {};
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bd.MiscFlags = 0;
    bd.ByteWidth = sizeof(MVP); 
    bd.StructureByteStride = 0u;

    DirectX::XMStoreFloat4x4(&_model, model);
    DirectX::XMStoreFloat4x4(&_view, view);
    DirectX::XMStoreFloat4x4(&_proj, proj);

    D3D11_SUBRESOURCE_DATA sd {};
    DirectX::XMStoreFloat4x4(&_mvp._mv, DirectX::XMMatrixMultiply(model, view));
    DirectX::XMStoreFloat4x4(&_mvp._mvp, DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&_mvp._mv), proj));
    sd.pSysMem = &_mvp;

    device.CreateBuffer(&bd, &sd, _resource.ReleaseAndGetAddressOf());
}

void Engine::Graphics::D3D11TransformMVP::SetModel(DirectX::XMMATRIX const& model) {
    DirectX::XMStoreFloat4x4(&_model, model);
}

void Engine::Graphics::D3D11TransformMVP::SetView(DirectX::XMMATRIX const& view) {
    DirectX::XMStoreFloat4x4(&_view, view);
}

void Engine::Graphics::D3D11TransformMVP::SetProj(DirectX::XMMATRIX const& proj) {
    DirectX::XMStoreFloat4x4(&_proj, proj);
}

void Engine::Graphics::D3D11TransformMVP::Update(ID3D11DeviceContext& context) {
    using namespace DirectX;
    XMStoreFloat4x4(&_mvp._mv, XMMatrixMultiply(XMLoadFloat4x4(&_model), XMLoadFloat4x4(&_view)));
    XMStoreFloat4x4(&_mvp._mvp, XMMatrixMultiply(XMLoadFloat4x4(&_mvp._mv), XMLoadFloat4x4(&_proj)));

    D3D11_MAPPED_SUBRESOURCE msr {};
    context.Map(
        _resource.Get(),
        0u,
        D3D11_MAP_WRITE_DISCARD,
        0u,
        &msr
    );
    memcpy(msr.pData, &_mvp, sizeof(MVP));
    context.Unmap(_resource.Get(), 0u);
}

void Engine::Graphics::D3D11TransformMVP::Bind(ID3D11DeviceContext& context) const {
    context.VSSetConstantBuffers(0u, 1u, _resource.GetAddressOf());
}