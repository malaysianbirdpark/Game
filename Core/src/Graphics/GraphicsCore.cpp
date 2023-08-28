#include "pch.h"
#include "GraphicsCore.h"

#include "D3D11/D3D11Core.h"
//#include "D3D12/D3D12Core.h"
#include <imgui.h>

#include "D3DCamera.h"
#include "GUI/D3D11ImGuiRenderer.h"

//Engine::Graphics::GraphicsCore::GraphicsCore(int width, int height, HWND nativeWnd, bool windowed) {
//    _core11 = MakeUnique<D3D11Core>(width, height, nativeWnd, windowed);
//    //_core12 = MakeUnique<D3D12Core>(width, height, nativeWnd, windowed);
//    _cam = MakeUnique<D3DCamera>();
//
//}
//
//Engine::Graphics::GraphicsCore::~GraphicsCore() {
//}
//
//void Engine::Graphics::GraphicsCore::Update(float const dt) {
//    _core11->Update(dt);
//}
//
//void Engine::Graphics::GraphicsCore::BeginFrame() {
//    _core11->BeginFrame();
//    D3D11ImGuiRenderer::BeginFrame();
//}
//
//void Engine::Graphics::GraphicsCore::EndFrame() {
//    D3D11ImGuiRenderer::EndFrame();
//    _core11->EndFrame();
//}
//
//ID3D11Device& Engine::Graphics::GraphicsCore::Device() {
//    return _core11->Device();
//}
