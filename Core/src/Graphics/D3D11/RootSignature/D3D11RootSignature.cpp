#include "pch.h"
#include "D3D11RootSignature.h"

#include "D3D11RootSignatureHolder.h"
#include "D3D11RootSignatureCommon.h"
#include "Graphics/D3D11/PipelineState/D3D11PipelineStateObject.h"
#include "../../D3DCamera.h"

Engine::Graphics::D3D11RootSignature::D3D11RootSignature(ID3D11Device& device, DirectX::XMMATRIX const& proj, std::shared_ptr<D3D11PipelineStateObject> const& target_pso, uint16_t priority)
    : _targetPSO{target_pso}, _priority{priority} 
{
    DirectX::XMStoreFloat4x4(&_mvp.model, DirectX::XMMatrixIdentity());
    DirectX::XMStoreFloat4x4(&_mvp.view, D3DCamera::GetView());
    DirectX::XMStoreFloat4x4(&_mvp.projection, proj);
    _cb[0] = MakeShared<D3D11ConstantBuffer>(device, &_mvp, sizeof(MVP), true, false, true, "mvp");
}

void Engine::Graphics::D3D11RootSignature::AddConstantBuffer(ID3D11Device& device, UINT slot, void* data, size_t data_size, bool vsVisible, bool psVisible, bool dynamic, char const* tag) {
    CORE_ASSERT(slot != 0, "Slot 0 for constant-buffer is reserved for the mvp matrices!");
    _cb[slot] = D3D11RootSignatureHolder::ResolveConstantBuffer(device, data, data_size, vsVisible, psVisible, dynamic, tag);
}

void Engine::Graphics::D3D11RootSignature::AddSamplerSlot(ID3D11Device& device, UINT slot, char const* tag) {
    _sampler[slot] = D3D11RootSignatureHolder::ResolveSampler(device, tag);
}

void Engine::Graphics::D3D11RootSignature::AddShaderResourceSlot(ID3D11Device& device, UINT slot, char const* path) {
    _texture[slot] = D3D11RootSignatureHolder::ResolveTexture(device, path);
}

void Engine::Graphics::D3D11RootSignature::UpdateModel(DirectX::XMMATRIX const& model) {
    DirectX::XMStoreFloat4x4(&_mvp.model, model);
}

void Engine::Graphics::D3D11RootSignature::UpdateView(DirectX::XMMATRIX const& view) {
    DirectX::XMStoreFloat4x4(&_mvp.view, view);
}

void Engine::Graphics::D3D11RootSignature::UpdateProj(DirectX::XMMATRIX const& proj) {
    DirectX::XMStoreFloat4x4(&_mvp.projection, proj);
}

void Engine::Graphics::D3D11RootSignature::Update(ID3D11DeviceContext& context) {
    UpdateView(D3DCamera::GetView());
    _cb[0]->Update(context, &_mvp, sizeof(MVP));
}

void Engine::Graphics::D3D11RootSignature::Bind(ID3D11DeviceContext& context) {
    _targetPSO->Bind(context);
    for (auto& cb : _cb)
        cb.second->Bind(context, cb.first);
    for (auto& s : _sampler)
        s.second->Bind(context, s.first);
    for (auto& t : _texture)
        t.second->Bind(context, t.first);
}
