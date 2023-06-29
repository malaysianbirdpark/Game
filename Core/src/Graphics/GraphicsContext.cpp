#include "pch.h"
#include "GraphicsContext.h"

#include "D3D11/D3D11Core.h"
#include "D3D12/D3D12Core.h"

Engine::Graphics::GraphicsContext::GraphicsContext(int width, int height, HWND nativeWnd, bool windowed) {
    _core11 = MakeUnique<D3D11Core>(width, height, nativeWnd, windowed);
    //_core12 = MakeUnique<D3D12Core>(width, height, nativeWnd, windowed);
}

Engine::Graphics::GraphicsContext::~GraphicsContext() {
}

void Engine::Graphics::GraphicsContext::Render() {
    _core11->Render();
    //_core12->Render();
}
