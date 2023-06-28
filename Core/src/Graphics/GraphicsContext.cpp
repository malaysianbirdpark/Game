#include "pch.h"
#include "GraphicsContext.h"

#include "D3D12/D3D12Core.h"

Engine::Graphics::GraphicsContext::GraphicsContext(int width, int height, HWND nativeWnd, bool windowed) {
    _graphicsCore = MakeUnique<D3D12Core>(width, height, nativeWnd, windowed);
}

Engine::Graphics::GraphicsContext::~GraphicsContext() {
}

void Engine::Graphics::GraphicsContext::Render() {
    _graphicsCore->Render();
}
