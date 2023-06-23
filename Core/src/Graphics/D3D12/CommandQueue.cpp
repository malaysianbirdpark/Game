#include "pch.h"
#include "CommandQueue.h"

#include "Graphics/GraphicsContext.h"
#include "GraphicsDevice.h"

Engine::Graphics::CommandQueue::CommandQueue(GraphicsContext& gfx, D3D12_COMMAND_LIST_TYPE type)
    : _type{type}
{
    D3D12_COMMAND_QUEUE_DESC qd {};
    qd.Type = _type;
    qd.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

    ThrowIfFailed(DEVICE().CreateCommandQueue(&qd, IID_PPV_ARGS(_cmdQueue.ReleaseAndGetAddressOf())));
}
