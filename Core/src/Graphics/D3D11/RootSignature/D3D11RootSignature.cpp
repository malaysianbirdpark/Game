#include "pch.h"
#include "D3D11RootSignature.h"

#include "D3D11RootSignatureHolder.h"
#include "D3D11RootSignatureCommon.h"
#include "Graphics/D3D11/PipelineState/D3D11PipelineStateObject.h"
#include "../../D3DCamera.h"

#include "ConstantBuffer/D3D11ConstantBuffer.h"
#include "ShaderResource/D3D11ShaderResource.h"

Engine::Graphics::D3D11RootSignature::D3D11RootSignature(ID3D11Device& device, std::shared_ptr<D3D11PipelineStateObject> const& target_pso, uint16_t priority)
    : _targetPSO{target_pso}, _priority{priority} 
{
}

void Engine::Graphics::D3D11RootSignature::Bind(ID3D11DeviceContext& context) {
    _targetPSO->Bind(context);
    for (auto const& cb : _cbs)
        std::visit(BindConstantBuffer{context}, *cb);
}
