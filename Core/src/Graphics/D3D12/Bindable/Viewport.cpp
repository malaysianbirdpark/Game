#include "pch.h"
#include "Viewport.h"

#include "Graphics/GraphicsContext.h"
#include "Graphics/D3D12/Commander.h"

Engine::Graphics::Viewport::Viewport(float width, float height, float top_left_x, float top_left_y, float min_depth, float max_depth)
    : Bindable{}
{
    _vp.Width = width;
    _vp.Height = height;
    _vp.TopLeftX = top_left_x;
    _vp.TopLeftY = top_left_y;
    _vp.MinDepth = min_depth;
    _vp.MaxDepth = max_depth;
}

Engine::Graphics::Viewport::~Viewport() {
}

void Engine::Graphics::Viewport::Bind(GraphicsContext& gfx) noexcept {
    CMD_LIST().RSSetViewports(1u, &_vp);
}

std::string Engine::Graphics::Viewport::GetUID() const noexcept {
    return "";
}
