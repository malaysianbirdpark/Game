#include "pch.h"
#include "PipelineState.h"

#include "Graphics/GraphicsContext.h"
#include "Graphics/D3D12/Commander.h"
#include "Graphics/D3D12/RootSignature.h"

Engine::Graphics::PipelineState::PipelineState(GraphicsContext& gfx) {
	// TODO: input element desc
    D3D12_INPUT_ELEMENT_DESC desc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	// TODO: passing root-signature
	_desc.InputLayout = { desc, _countof(desc) };
	_desc.pRootSignature = ROOT_SIGNATURE().Get();

	_desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	_desc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	_desc.DepthStencilState.DepthEnable = FALSE;
	_desc.DepthStencilState.StencilEnable = FALSE;
	_desc.SampleMask = UINT_MAX;
	_desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	_desc.NumRenderTargets = 1;
	_desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	_desc.SampleDesc.Count = 1;
}

void Engine::Graphics::PipelineState::Bind(GraphicsContext& gfx) noexcept {
    CMD_LIST().SetPipelineState(_pipelineState.Get());
}

std::string Engine::Graphics::PipelineState::GetUID() const noexcept {
    return Bindable::GetUID();
}
