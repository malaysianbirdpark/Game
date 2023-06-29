#include "pch.h"
#include "PipelineState.h"

#include "Graphics/GraphicsContext.h"
#include "Graphics/D3D12/Bindable/PipelineState/PixelShader.h"
#include "Graphics/D3D12/Bindable/PipelineState/VertexShader.h"
#include "Graphics/D3D12/Bindable/RootSignature/RootSignature.h"

Engine::Graphics::PipelineState::PipelineState(GraphicsContext& gfx, std::unique_ptr<RootSignature>& p_root_signature, std::unique_ptr<VertexShader>& p_vertex_shader, std::unique_ptr<PixelShader>& p_pixel_shader) {
	// TODO: input element desc
    D3D12_INPUT_ELEMENT_DESC input_element_dest[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	// TODO: passing root-signature
	_desc.InputLayout = { input_element_dest, _countof(input_element_dest) };
	_desc.pRootSignature = p_root_signature->GetSignature().Get(); 

	_desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	_desc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	_desc.DepthStencilState.DepthEnable = FALSE;
	_desc.DepthStencilState.StencilEnable = FALSE;
	_desc.SampleMask = UINT_MAX;
	_desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	_desc.NumRenderTargets = 1;
	_desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	_desc.SampleDesc.Count = 1;

	_desc.VS = p_vertex_shader->GetByteCode();
	_desc.PS = p_pixel_shader->GetByteCode();

	DEVICE().CreateGraphicsPipelineState(&_desc, IID_PPV_ARGS(_pipelineState.ReleaseAndGetAddressOf()));
}

void Engine::Graphics::PipelineState::Bind(GraphicsContext& gfx) noexcept {
    CMD_LIST().SetPipelineState(_pipelineState.Get());
}

std::string Engine::Graphics::PipelineState::GetUID() const noexcept {
    return Bindable::GetUID();
}
