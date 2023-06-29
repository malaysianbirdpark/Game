#include "gwpch.h"
#include "D3D11TransformCbuf.h"

#include "../Drawable/D3D11Drawable.h"

namespace Glowing::Bind {
	TransformCbuf::TransformCbuf(D3D11Drawable const& parent, UINT slot)
        : parent{parent}
    {
        if (!pVcbuf)
            pVcbuf = std::make_unique<VertexConstantBuffer<Transforms>>(slot);
    }

    void TransformCbuf::Bind() noexcept {
        UpdateBindImpl(GetTransforms());
    }

    void TransformCbuf::UpdateBindImpl(Transforms const& tf) noexcept {
        pVcbuf->Update(tf);
        pVcbuf->Bind();
    }

    TransformCbuf::Transforms TransformCbuf::GetTransforms() noexcept {
        auto const modelView {parent.GetTransformXM() * RenderDevice::GetView()};
        Transforms const tf {
            DirectX::XMMatrixTranspose(modelView),
            DirectX::XMMatrixTranspose(modelView * RenderDevice::GetProj())
        };
        return tf;
    }
}
