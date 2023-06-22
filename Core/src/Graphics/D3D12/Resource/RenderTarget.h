#pragma once
#include "Resource.h"

namespace Engine::Graphics {
    class GraphicsContext;
    class Resource;

    class RenderTarget {
        friend class SwapChain;
    public:
        RenderTarget(GraphicsContext& gfx, UINT width, UINT height, DXGI_FORMAT format);
        ~RenderTarget() = default;

        Microsoft::WRL::ComPtr<ID3D12Resource> GetResource() const { return p_resource->GetResource(); }
    private:
        std::unique_ptr<Resource> p_resource {};
    };
}

