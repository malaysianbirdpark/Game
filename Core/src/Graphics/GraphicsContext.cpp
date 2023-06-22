#include "pch.h"
#include "GraphicsContext.h"

#include "Graphics/D3D12/GraphicsDevice.h"
#include "Graphics/D3D12/CommandQueue.h"
#include "Graphics/D3D12/Commander.h"
#include "Graphics/D3D12/Fence.h"
#include "Graphics/D3D12/SwapChain.h"
//#include "Graphics/DirectX12/Bindable/DescHeap/RenderTargetView.h"
//#include "Graphics/D3D12/Bindable/Viewport.h"
//#include "Graphics/D3D12/Bindable/ScissorRect.h"

Engine::Graphics::GraphicsContext::GraphicsContext(int width, int height, HWND nativeWnd, bool windowed)
    : _windowData{width, height, nativeWnd, windowed}
{
    p_device = MakeUnique<GraphicsDevice>();
    p_cmdQueue = MakeUnique<CommandQueue>(*this, D3D12_COMMAND_LIST_TYPE_DIRECT);
    p_commander = MakeUnique<Commander>(*this, D3D12_COMMAND_LIST_TYPE_DIRECT);
    p_fence = MakeUnique<Fence>(*this);
    p_swapChain = MakeUnique<SwapChain>(*this);
    //p_RTV = std::make_unique<RenderTargetView>(p_device, p_swapChain, 3);
    //p_vp = std::make_unique<Viewport>(width, height);
    //p_rect = std::make_unique<ScissorRect>(0, 0, width, height);
}

Engine::Graphics::GraphicsContext::~GraphicsContext() {
}

std::unique_ptr<Engine::Graphics::GraphicsDevice>& Engine::Graphics::GraphicsContext::GetDeviceImpl() {
    return p_device;
}

std::unique_ptr<Engine::Graphics::CommandQueue>& Engine::Graphics::GraphicsContext::GetCommandQueueImpl() {
    return p_cmdQueue;
}

std::unique_ptr<Engine::Graphics::SwapChain>& Engine::Graphics::GraphicsContext::GetSwapChainImpl() {
    return p_swapChain;
}

Engine::Graphics::WindowData& Engine::Graphics::GraphicsContext::GetWindowData() {
    return _windowData;
}

void Engine::Graphics::GraphicsContext::Render() {
    BeginFrame();
    EndFrame();
}

void Engine::Graphics::GraphicsContext::BeginFrame() {
    p_commander->Reset();

    p_swapChain->BeginFrame(p_commander);

    //p_vp->Bind(*this);
    //p_rect->Bind(*this);

    ClearScreen();
}

void Engine::Graphics::GraphicsContext::EndFrame() {
    p_swapChain->EndFrame();
    p_commander->Close();

    ExecuteCommand();
}

void Engine::Graphics::GraphicsContext::ExecuteCommand() {
    ID3D12CommandList* cmdList[] {p_cmdQueue->_cmdList.Get()};
    p_cmdQueue->_cmdQueue->ExecuteCommandLists(_countof(cmdList), cmdList);

    p_swapChain->Present();

    p_fence->SetSignalNext(p_cmdQueue);
    p_fence->Wait();

    p_swapChain->Swap();
}

void Engine::Graphics::GraphicsContext::ClearScreen() {
    //p_RTV->Clear(*this);
    //p_RTV->Bind(*this);
}
