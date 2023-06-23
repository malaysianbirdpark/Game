#include "pch.h"
#include "ScissorRect.h"

#include "Graphics/GraphicsContext.h"
#include "Graphics/D3D12/Commander.h"

Engine::Graphics::ScissorRect::ScissorRect(long left, long top, long right, long bottom)
    : Bindable{}
{
    _rect = CD3DX12_RECT{left, top, right, bottom};
}

Engine::Graphics::ScissorRect::~ScissorRect() {
}

void Engine::Graphics::ScissorRect::Bind(GraphicsContext& gfx) noexcept {
    CMD_LIST().RSSetScissorRects(1u, &_rect);
}

std::string Engine::Graphics::ScissorRect::GetUID() const noexcept {
    return "";
}
