#include "pch.h"
#include "GraphicsContext.h"

#include "D3D11/D3D11Core.h"
//#include "D3D12/D3D12Core.h"
#include "D3DCamera.h"
#include "D3DSceneGraph.h"

Engine::Graphics::GraphicsContext::GraphicsContext(int width, int height, HWND nativeWnd, bool windowed) {
    _core11 = MakeUnique<D3D11Core>(width, height, nativeWnd, windowed);
    //_core12 = MakeUnique<D3D12Core>(width, height, nativeWnd, windowed);
    _cam = MakeUnique<D3DCamera>();
}

Engine::Graphics::GraphicsContext::~GraphicsContext() {
}

void Engine::Graphics::GraphicsContext::Update(float const dt, DirectX::XMMATRIX view_projection) {

}

void Engine::Graphics::GraphicsContext::Render(D3DSceneGraph& scene) {
    _core11->Render(scene);
    //_core12->Render();
}
