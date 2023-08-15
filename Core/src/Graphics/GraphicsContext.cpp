#include "pch.h"
#include "GraphicsContext.h"

#include "D3D11/D3D11Core.h"
//#include "D3D12/D3D12Core.h"
#include <imgui.h>

#include "D3DCamera.h"
#include "GUI/D3D11ImGuiRenderer.h"

Engine::Graphics::GraphicsContext::GraphicsContext(int width, int height, HWND nativeWnd, bool windowed) {
    _core11 = MakeUnique<D3D11Core>(width, height, nativeWnd, windowed);
    //_core12 = MakeUnique<D3D12Core>(width, height, nativeWnd, windowed);
    _cam = MakeUnique<D3DCamera>();

}

Engine::Graphics::GraphicsContext::~GraphicsContext() {
}

void Engine::Graphics::GraphicsContext::Update(float const dt) {
    _core11->Update(dt, _cam->GetView());
}

void Engine::Graphics::GraphicsContext::BeginFrame() {
    _core11->BeginFrame();
    D3D11ImGuiRenderer::BeginFrame();
}

void Engine::Graphics::GraphicsContext::EndFrame() {
    D3D11ImGuiRenderer::EndFrame();
    _core11->EndFrame();
}

ID3D11Device& Engine::Graphics::GraphicsContext::Device() {
    return _core11->Device();
}
