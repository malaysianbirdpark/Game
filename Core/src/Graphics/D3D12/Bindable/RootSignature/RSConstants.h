#pragma once

#include "RootSignature.h"

#include "Graphics/GraphicsContext.h"
#include "Graphics/D3D12/Commander.h"

namespace Engine::Graphics {
class GraphicsContext;

template <typename T>
class RSConstants : public RootSignature {
public:
    RSConstants(GraphicsContext& gfx, T data, UINT slot, UINT num);
    virtual ~RSConstants() override = default;

    void Bind(GraphicsContext& gfx) noexcept override;
    [[nodiscard]] std::string GetUID() const noexcept override;
private:
    T _data;
};
}

template <typename T>
Engine::Graphics::RSConstants<T>::RSConstants(GraphicsContext& gfx, T data, UINT slot, UINT num)
    : RootSignature{
        gfx,
        [num, slot] () {
            auto param {new CD3DX12_ROOT_PARAMETER{}};
            param[0].InitAsConstants(sizeof(T) / sizeof(DWORD), slot, 0, D3D12_SHADER_VISIBILITY_VERTEX);

            D3D12_ROOT_SIGNATURE_FLAGS constexpr flag {
                D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT    |
                D3D12_ROOT_SIGNATURE_FLAG_DENY_MESH_SHADER_ROOT_ACCESS          |
                D3D12_ROOT_SIGNATURE_FLAG_DENY_AMPLIFICATION_SHADER_ROOT_ACCESS |
                D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS        |
                D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS      |
                D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS         
            };

            return CD3DX12_ROOT_SIGNATURE_DESC(
                1u,
                param,
                0,
                nullptr,
                flag
            );
        },
        slot,
        num
    }, _data{data}
{
    
}

template <typename T>
void Engine::Graphics::RSConstants<T>::Bind(GraphicsContext& gfx) noexcept {
    CMD_LIST().SetGraphicsRootSignature(_signature.Get());
    CMD_LIST().SetComputeRoot32BitConstants(0u, sizeof(T) / sizeof(DWORD), reinterpret_cast<void*>(&_data), 0);
}

template <typename T>
std::string Engine::Graphics::RSConstants<T>::GetUID() const noexcept {
    return RootSignature::GetUID();
}

