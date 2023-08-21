#include "pch.h"
#include "D3D11EnvironmentMappingReflectStrategy.h"

#include "Graphics/D3D11/PipelineState/D3D11PSOLibrary.h"
#include "Graphics/D3D11/PipelineState/D3D11PipelineStateObject.h"

#include "Graphics/D3D11/SceneGraph/D3D11SceneGraph.h"

#include "Graphics/D3D11/D3D11CubeMap.h"


void Engine::Graphics::D3D11EnvironmentMappingReflectStrategy::Init() {
    GRAPHICS_INFO("Environment Mapping Strategy Init()");
    _pso = D3D11PSOLibrary::ResolvePSO("environment_mapping_reflect");
}

void Engine::Graphics::D3D11EnvironmentMappingReflectStrategy::Render(ID3D11DeviceContext& context, D3D11Mesh const& mesh, D3D11Material const& material) {
    mesh.Bind(context);
    _pso->Bind(context);

    D3D11CubeMap::BindSpecular(context);

    context.DrawIndexed(mesh.GetIndexCount(), 0u, 0u);
}
