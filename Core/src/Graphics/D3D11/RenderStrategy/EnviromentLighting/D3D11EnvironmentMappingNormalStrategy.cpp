#include "pch.h"
#include "D3D11EnvironmentMappingNormalStrategy.h"

#include "Graphics/D3D11/PipelineState/D3D11PSOLibrary.h"
#include "Graphics/D3D11/PipelineState/D3D11PipelineStateObject.h"

#include "Graphics/D3D11/SceneGraph/D3D11SceneGraph.h"

#include "Graphics/D3D11/D3D11Cubemap.h"

void Engine::Graphics::D3D11EnvironmentMappingNormalStrategy::Init() {
    GRAPHICS_INFO("Environment Mapping Strategy Init()");
    _pso = D3D11PSOLibrary::ResolvePSO("environment_mapping_normal");
}

void Engine::Graphics::D3D11EnvironmentMappingNormalStrategy::Render(ID3D11DeviceContext& context, ID3D11ShaderResourceView* const* srv, D3D11Mesh const& mesh, D3D11Material& material) {
    mesh.Bind(context);
    _pso->Bind(context);

    D3D11Cubemap::Bind(context);

    context.PSSetShaderResources(30u, 1u, srv);
    context.DrawIndexed(mesh.GetIndexCount(), 0u, 0u);
}

