#include "pch.h"
#include "GraphicsContext.h"

#include "Graphics/D3D12/GraphicsDevice.h"
#include "Graphics/D3D12/CommandQueue.h"
#include "Graphics/D3D12/Commander.h"
#include "Graphics/D3D12/Fence.h"
#include "Graphics/D3D12/SwapChain.h"
#include "Graphics/D3D12/Descriptor/RenderTargetView.h"
#include "Graphics/D3D12/Bindable/Viewport.h"
#include "Graphics/D3D12/Bindable/ScissorRect.h"
#include "Graphics/D3D12/Bindable/RootSignature.h"

#include "Graphics/D3D12/Bindable/VertexBuffer.h"
#include "Graphics/D3D12/VertexShader.h"
#include "Graphics/D3D12/PixelShader.h"
#include "Graphics/D3D12/Bindable/PipelineState.h"

#include "Graphics/D3D12/Drawable/Triangle.h"

Engine::Graphics::GraphicsContext::GraphicsContext(int width, int height, HWND nativeWnd, bool windowed)
    : _windowData{width, height, nativeWnd, windowed}
{
    p_device = MakeUnique<GraphicsDevice>();
    p_cmdQueue = MakeUnique<CommandQueue>(*this, D3D12_COMMAND_LIST_TYPE_DIRECT);
    p_commander = MakeUnique<Commander>(*this, D3D12_COMMAND_LIST_TYPE_DIRECT);
    p_fence = MakeUnique<Fence>(*this);
    p_swapChain = MakeUnique<SwapChain>(*this);
    p_RTV = MakeUnique<RenderTargetView>(*this, p_swapChain->GetNumOfBuffers());
    p_vp = MakeUnique<Viewport>(width, height);
    p_rect = MakeUnique<ScissorRect>(0, 0, width, height);
    p_rootSignature = MakeUnique<RootSignature>(*this);

    p_vertexShader = MakeUnique<VertexShader>(L"./ShaderLib/basicVS.cso");
    p_pixelShader = MakeUnique<PixelShader>(L"./ShaderLib/basicPS.cso");
    p_pipelineState = MakeUnique<PipelineState>(*this, p_rootSignature, p_vertexShader, p_pixelShader);

    p_triangle = MakeUnique<Triangle>(*this);
}

Engine::Graphics::GraphicsContext::~GraphicsContext() {
    p_commander->Close();
    p_commander->Reset();
}

std::unique_ptr<Engine::Graphics::GraphicsDevice>& Engine::Graphics::GraphicsContext::GetDeviceImpl() {
    return p_device;
}

std::unique_ptr<Engine::Graphics::CommandQueue>& Engine::Graphics::GraphicsContext::GetCommandQueueImpl() {
    return p_cmdQueue;
}

std::unique_ptr<Engine::Graphics::Commander>& Engine::Graphics::GraphicsContext::GetCommander() {
    return p_commander;
}

std::unique_ptr<Engine::Graphics::RootSignature>& Engine::Graphics::GraphicsContext::GetRootSignature() {
    return p_rootSignature;
}

std::unique_ptr<Engine::Graphics::SwapChain>& Engine::Graphics::GraphicsContext::GetSwapChainImpl() {
    return p_swapChain;
}

Engine::Graphics::WindowData& Engine::Graphics::GraphicsContext::GetWindowData() {
    return _windowData;
}

void Engine::Graphics::GraphicsContext::Render() {
    BeginRecord();
    EndRecord();
}

void Engine::Graphics::GraphicsContext::BeginRecord() {
    // TODO: Multi-thread

    // 1. Reset the Command List
    p_commander->Reset();

    p_swapChain->BeginFrame(p_commander->GetCmdList());

    ClearScreen();
    
    // 2. Bind the RootSignature
    p_rootSignature->Bind(*this);

    // 3. Bind the PipelineStateObject
    p_pipelineState->Bind(*this);

    // 4. Bind Vp, scissor, RenderTarget, Vertex, topology, root constants
    p_vp->Bind(*this);
    p_rect->Bind(*this);
}

void Engine::Graphics::GraphicsContext::EndRecord() {
    // TODO: Multi-thread
    p_triangle->Draw(*this);
    
    p_swapChain->EndFrame(p_commander->GetCmdList());

    p_commander->Close();

    ExecuteCommand();

}

void Engine::Graphics::GraphicsContext::ExecuteCommand() {
    // TODO: Multi-thread
    ID3D12CommandList* cmd_list[] {&p_commander->GetCmdList()};
    p_cmdQueue->GetQueue().ExecuteCommandLists(1u, cmd_list);

    p_fence->SetSignalNext(*this);

    p_swapChain->Present();

    p_fence->Wait();

    p_swapChain->Swap();
}

void Engine::Graphics::GraphicsContext::ClearScreen() {
    // TODO: Job System
    static float color[] {0.24f, 0.12f, 0.64f, 1.0f};
    auto const back_buffer_view {p_RTV->GetHandle(p_swapChain->GetCurrentBufferIndex())};
    p_commander->GetCmdList().OMSetRenderTargets(1u, &back_buffer_view, FALSE, nullptr);
    p_commander->GetCmdList().OMSetRenderTargets(1u, &back_buffer_view, FALSE, nullptr);
    p_commander->GetCmdList().ClearRenderTargetView(back_buffer_view, color, 0u, nullptr);
}
