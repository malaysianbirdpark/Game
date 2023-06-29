#pragma once

#include "D3D11Bindable.h"
#include "BindableCodex.h"

#include "Renderer/RenderDevice.h"

namespace Glowing::Bind {
    template<typename C>
    class ConstantBuffer : public Bindable {
    public:
        ConstantBuffer(UINT slot = 0u);
        ConstantBuffer(C const& consts, UINT slot = 0u);

        void Update(C const& consts);
        void SetSlot(UINT slot) { this->slot = slot; }
    protected:
        Microsoft::WRL::ComPtr<ID3D11Buffer> p_ConstantBuffer;
        UINT slot;
    };

    template<typename C>
    class VertexConstantBuffer : public ConstantBuffer<C> {
    public:
        VertexConstantBuffer(UINT slot = 0u)
            : ConstantBuffer<C>{slot}
        {
        }
        
        VertexConstantBuffer(C const& consts, UINT slot = 0u)
            : ConstantBuffer<C>{consts, slot}
        {
        }

        void Bind() noexcept override;

        [[nodiscard]] static std::shared_ptr<VertexConstantBuffer> Resolve(C const& consts, UINT slot = 0u);
        [[nodiscard]] static std::shared_ptr<VertexConstantBuffer> Resolve(UINT slot = 0u);
        [[nodiscard]] static std::string GenerateUID(C const&, UINT slot = 0u);
        [[nodiscard]] static std::string GenerateUID(UINT slot = 0u);
        [[nodiscard]] std::string GetUID() const noexcept override;
    };

    template<typename C>
    class GLOWING_API PixelConstantBuffer final : public ConstantBuffer<C> {
    public:
        PixelConstantBuffer(UINT slot = 0u)
            : ConstantBuffer<C>{slot}
        {
        }
        
        PixelConstantBuffer(C const& consts, UINT slot = 0u)
            : ConstantBuffer<C>{consts, slot}
        {
        }

        void Bind() noexcept override;

        [[nodiscard]] static std::shared_ptr<PixelConstantBuffer> Resolve(C const& consts, UINT slot = 0u);
        [[nodiscard]] static std::shared_ptr<PixelConstantBuffer> Resolve(UINT slot = 0u);
        [[nodiscard]] static std::string GenerateUID(C const&, UINT slot = 0u);
        [[nodiscard]] static std::string GenerateUID(UINT slot = 0u);
        [[nodiscard]] std::string GetUID() const noexcept override;
    };

    template <typename C>
    ConstantBuffer<C>::ConstantBuffer( UINT slot)
        : slot{slot}
    {
        D3D11_BUFFER_DESC bd {};
        bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        bd.Usage = D3D11_USAGE_DYNAMIC;
        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        bd.MiscFlags = 0;
        bd.ByteWidth = sizeof(C);
        bd.StructureByteStride = 0u;
        GET_DEVICE->CreateBuffer(&bd, nullptr, p_ConstantBuffer.ReleaseAndGetAddressOf());
    }

    template <typename C>
    ConstantBuffer<C>::ConstantBuffer(C const& consts, UINT slot)
        : slot{slot}
    {
        D3D11_BUFFER_DESC bd {};
        bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        bd.Usage = D3D11_USAGE_DYNAMIC;
        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        bd.MiscFlags = 0;
        bd.ByteWidth = sizeof(C);
        bd.StructureByteStride = 0u;

        D3D11_SUBRESOURCE_DATA sd {};
        sd.pSysMem = &consts;

        GET_DEVICE->CreateBuffer(&bd, &sd, p_ConstantBuffer.ReleaseAndGetAddressOf());
    }

    template <typename C>
    void ConstantBuffer<C>::Update(C const& consts) {
        D3D11_MAPPED_SUBRESOURCE msr;
        GET_CONTEXT->Map(
            p_ConstantBuffer.Get(),
            0u,
            D3D11_MAP_WRITE_DISCARD,
            0u,
            &msr
        );
        memcpy(msr.pData, &consts, sizeof(consts));
        GET_CONTEXT->Unmap(p_ConstantBuffer.Get(), 0u);
    }

    template <typename C>
    std::shared_ptr<VertexConstantBuffer<C>> VertexConstantBuffer<C>::Resolve(C const& consts, UINT slot) {
        return Codex::Resolve<VertexConstantBuffer>(consts, slot);
    }

    template <typename C>
    std::shared_ptr<VertexConstantBuffer<C>> VertexConstantBuffer<C>::Resolve(UINT slot) {
        return Codex::Resolve<VertexConstantBuffer>(slot);
    }

    template <typename C>
    std::string VertexConstantBuffer<C>::GenerateUID(C const&, UINT slot) {
        return GenerateUID(slot);
    }

    template <typename C>
    std::string VertexConstantBuffer<C>::GenerateUID(UINT slot) {
        using namespace std::string_literals;
        return typeid(VertexConstantBuffer).name() + "#"s + std::to_string(slot);
    }

    template <typename C>
    std::string VertexConstantBuffer<C>::GetUID() const noexcept {
        return GenerateUID(this->slot);
    }

    template <typename C>
    void VertexConstantBuffer<C>::Bind() noexcept {
        GET_CONTEXT->VSSetConstantBuffers(this->slot, 1u, this->p_ConstantBuffer.GetAddressOf());
    }

    template <typename C>
    void PixelConstantBuffer<C>::Bind() noexcept {
        GET_CONTEXT->PSSetConstantBuffers(this->slot, 1u, this->p_ConstantBuffer.GetAddressOf());
    }

    template <typename C>
    std::shared_ptr<PixelConstantBuffer<C>> PixelConstantBuffer<C>::Resolve(C const& consts, UINT slot) {
        return Codex::Resolve<PixelConstantBuffer>(consts, slot);
    }

    template <typename C>
    std::shared_ptr<PixelConstantBuffer<C>> PixelConstantBuffer<C>::Resolve(UINT slot) {
        return Codex::Resolve<PixelConstantBuffer>(slot);
    }

    template <typename C>
    std::string PixelConstantBuffer<C>::GenerateUID(C const&, UINT slot) {
        return GenerateUID(slot);
    }

    template <typename C>
    std::string PixelConstantBuffer<C>::GenerateUID(UINT slot) {
        using namespace std::string_literals;
        return typeid(PixelConstantBuffer).name() + "#"s + std::to_string(slot);
    }

    template <typename C>
    std::string PixelConstantBuffer<C>::GetUID() const noexcept {
        return GenerateUID(this->slot);
    }
}

