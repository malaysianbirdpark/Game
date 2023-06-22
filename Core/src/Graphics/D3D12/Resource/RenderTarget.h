#pragma once

namespace Engine::Graphics {
    class GraphicsContext;
    class Resource;

    class RenderTarget {
        friend class SwapChain;
    public:
        RenderTarget(GraphicsContext& gfx, UINT width, UINT height, DXGI_FORMAT format);
        ~RenderTarget() = default;
    private:
        std::unique_ptr<Resource> p_resource;
    };
}

