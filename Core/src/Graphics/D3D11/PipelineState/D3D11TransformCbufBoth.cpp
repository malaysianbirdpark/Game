#include "gwpch.h"
#include "D3D11TransformCbufBoth.h"

namespace Glowing::Bind {
    TransformCbufBoth::TransformCbufBoth(D3D11Drawable const& parent, UINT slotV, UINT slotP)
        : TransformCbuf(parent, slotV)
    {
        if (!pPcbuf)
            pPcbuf = std::make_unique<PixelConstantBuffer<Transforms>>(slotP);
    }

    void TransformCbufBoth::Bind() noexcept {
        auto const tf {GetTransforms()};
        TransformCbuf::UpdateBindImpl(tf);
        UpdateBindImpl(tf);
    }

    void TransformCbufBoth::UpdateBindImpl(Transforms const& tf) noexcept {
        pPcbuf->Update(tf);
        pPcbuf->Bind();
    }
}
