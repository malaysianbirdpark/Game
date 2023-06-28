#include "pch.h"
#include "RootSignature.h"

#include "Graphics/D3D12/GraphicsDevice.h"

Engine::Graphics::RootSignature& Engine::Graphics::RootSignature::AddConstantBuffer() {
    _params.push_back(std::move(CD3DX12_ROOT_PARAMETER{}));
    _params.back().InitAsConstantBufferView(_slot);
    //_elems.push_back(std::move(MakeUnique<RSConstantBuffer>()));
    ++_slot;

    return *this;
}

void Engine::Graphics::RootSignature::Cook(GraphicsContext& gfx) {
    auto const desc {GenerateDesc()};

    Microsoft::WRL::ComPtr<ID3DBlob> blob_signature;
    Microsoft::WRL::ComPtr<ID3DBlob> blob_error;
    if (FAILED(D3D12SerializeRootSignature(
        &desc,
        D3D_ROOT_SIGNATURE_VERSION_1_0,
        blob_signature.ReleaseAndGetAddressOf(),
        blob_error.ReleaseAndGetAddressOf()
    ))) 
    {
        if (blob_error) {
            GRAPHICS_ERROR(static_cast<char const*>(blob_error->GetBufferPointer()));
            CRASH();
        }
    }
    DEVICE().CreateRootSignature(
        0,
        blob_signature->GetBufferPointer(),
        blob_signature->GetBufferSize(),
        IID_PPV_ARGS(_signature.ReleaseAndGetAddressOf())
    );
}

void Engine::Graphics::RootSignature::Bind(GraphicsContext& gfx) noexcept {
    CMD_LIST().SetGraphicsRootSignature(_signature.Get());
    for (auto const& e : _elems)
        e->Bind(gfx);
}

std::string Engine::Graphics::RootSignature::GetUID() const noexcept {
    return Bindable::GetUID();
}

CD3DX12_ROOT_SIGNATURE_DESC Engine::Graphics::RootSignature::GenerateDesc() const {
    return CD3DX12_ROOT_SIGNATURE_DESC(
        _params.size(),
        _params.data(),
        0,
        nullptr,
        _flag
    );
}
