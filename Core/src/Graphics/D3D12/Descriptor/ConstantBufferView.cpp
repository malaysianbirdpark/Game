#include "pch.h"
#include "ConstantBufferView.h"

Engine::Graphics::ConstantBufferView::ConstantBufferView(GraphicsContext& gfx, UINT num_of_view)
    : DescriptorHeap{gfx, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, num_of_view}
{
}
