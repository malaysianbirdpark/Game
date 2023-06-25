#include "pch.h"
#include "RootSignature.h"

#include "Graphics/GraphicsContext.h"
#include "Graphics/D3D12/GraphicsDevice.h"
#include "Graphics/D3D12/Commander.h"

Engine::Graphics::RootSignature::RootSignature(GraphicsContext& gfx) {
    D3D12_ROOT_SIGNATURE_DESC desc {
        CD3DX12_ROOT_SIGNATURE_DESC(D3D12_DEFAULT)
    };
    desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    Microsoft::WRL::ComPtr<ID3DBlob> blob_signature;
    Microsoft::WRL::ComPtr<ID3DBlob> blob_error;
    D3D12SerializeRootSignature(
        &desc,
        D3D_ROOT_SIGNATURE_VERSION_1,
        blob_signature.ReleaseAndGetAddressOf(),
        blob_error.ReleaseAndGetAddressOf()
    );
    DEVICE().CreateRootSignature(
        0,
        blob_signature->GetBufferPointer(),
        blob_signature->GetBufferSize(),
        IID_PPV_ARGS(_signature.ReleaseAndGetAddressOf())
    );
}

void Engine::Graphics::RootSignature::Bind(GraphicsContext& gfx) noexcept {
    CMD_LIST().SetGraphicsRootSignature(_signature.Get());
}

std::string Engine::Graphics::RootSignature::GetUID() const noexcept {
    return Bindable::GetUID();
}
