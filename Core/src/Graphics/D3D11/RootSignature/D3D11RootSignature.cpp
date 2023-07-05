#include "pch.h"
#include "D3D11RootSignature.h"

#include "D3D11RootSignatureHolder.h"
#include "D3D11RootSignatureCommon.h"
#include "Graphics/D3D11/PipelineState/D3D11PipelineStateObject.h"

void Engine::Graphics::D3D11RootSignature::AddConstantBuffer(ID3D11Device& device, UINT slot, void* data, size_t data_size, bool vsVisible, bool psVisible, bool dynamic, char const* tag) {
    _cb[slot] = D3D11RootSignatureHolder::ResolveConstantBuffer(device, data, data_size, vsVisible, psVisible, dynamic, tag);
}

void Engine::Graphics::D3D11RootSignature::AddSampler(ID3D11Device& device, UINT slot, char const* tag) {
    _sampler[slot] = D3D11RootSignatureHolder::ResolveSampler(device, tag);
}

void Engine::Graphics::D3D11RootSignature::AddTexture(ID3D11Device& device, UINT slot, char const* path) {
    _texture[slot] = D3D11RootSignatureHolder::ResolveTexture(device, path);
}

void Engine::Graphics::D3D11RootSignature::UpdateMVP(ID3D11DeviceContext& context, void const* data, size_t data_size) {
    _cb[0]->Update(context, data, data_size);
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
