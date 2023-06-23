#include "pch.h"
#include "RenderTarget.h"

#include "Resource.h"

Engine::Graphics::RenderTarget::RenderTarget(GraphicsContext& gfx, UINT width, UINT height, DXGI_FORMAT format) {
    D3D12_RESOURCE_DESC desc {};
    desc.Format = format;
    desc.Width = width;
    desc.Height = height;
    desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
    desc.DepthOrArraySize = 1u;
    desc.SampleDesc.Count = 1u;
    desc.SampleDesc.Quality = 0;
    desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

    //p_resource = MakeUnique<Resource>(gfx, desc, D3D12_RESOURCE_STATE_RENDER_TARGET);
}
