#pragma once

#include "D3D11ConstantBuffer.h"

namespace Glowing {
    class D3D11Drawable;
}

namespace Glowing::Bind {
    class TransformCbuf : public Bindable {
    protected:
        struct Transforms {
            DirectX::XMMATRIX mv;
            DirectX::XMMATRIX mvp;
        };
    public:
        TransformCbuf(D3D11Drawable const& parent, UINT slot = 0u);
        void Bind() noexcept override;
    protected:
        void UpdateBindImpl(Transforms const& tf) noexcept;
        Transforms GetTransforms() noexcept;
    private:
        inline static std::unique_ptr<VertexConstantBuffer<Transforms>> pVcbuf;
        D3D11Drawable const& parent;
    };
}

