#include "pch.h"
#include "RootSignature.h"

#include "Graphics/D3D12/GraphicsDevice.h"
#include "Graphics/D3D12/Commander.h"

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
    for (auto& e : _elems)
        e->Bind(gfx);
}

std::string Engine::Graphics::RootSignature::GetUID() const noexcept {
    return Bindable::GetUID();
}

CD3DX12_ROOT_SIGNATURE_DESC Engine::Graphics::RootSignature::GenerateDesc() {
    return CD3DX12_ROOT_SIGNATURE_DESC(
        _params.size(),
        _params.data(),
        0,
        nullptr,
        _flag
    );
}
