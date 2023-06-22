#include "pch.h"
#include "RenderTargetView.h"

#include "Graphics/GraphicsContext.h"
#include "Graphics/D3D12/GraphicsDevice.h"
#include "Graphics/D3D12/SwapChain.h"

Engine::Graphics::RenderTargetView::RenderTargetView(GraphicsContext& gfx, UINT num_of_view)
    : DescriptorHeap{gfx, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, num_of_view}
{
	auto const rtvHeapBegin = _descHeap->GetCPUDescriptorHandleForHeapStart();
	for (int i = 0; i < num_of_view; i++)
	{
		_handle[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeapBegin, i * _heapSize);
		DEVICE().CreateRenderTargetView(SWAPCHAIN_IMPL()->GetBackBufferAt(i).Get(), nullptr, _handle[i]);
	}
}

