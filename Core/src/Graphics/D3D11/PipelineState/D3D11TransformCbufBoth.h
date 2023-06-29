#pragma once
#include "D3D11TransformCbuf.h"

namespace Glowing::Bind {
    class TransformCbufBoth : public TransformCbuf {
    public:
        TransformCbufBoth(D3D11Drawable const& parent, UINT slotV = 0u, UINT slotP = 0u);
        void Bind() noexcept override;
    protected:
        void UpdateBindImpl(Transforms const& tf) noexcept;
    private:
        inline static std::unique_ptr<PixelConstantBuffer<Transforms>> pPcbuf;
    };
}

