#include "pch.h"
#include "D3D11SolidStrategy.h"

#include "Graphics/D3D11/SceneGraph/D3D11SceneGraph.h"
#include "Graphics/D3D11/PipelineState/D3D11PSOLibrary.h"
#include "Graphics/D3D11/PipelineState/D3D11PipelineStateObject.h"

void Engine::Graphics::D3D11SolidStrategy::Init() {
    _pso = D3D11PSOLibrary::ResolvePSO("solid");
}

void Engine::Graphics::D3D11SolidStrategy::Render(ID3D11DeviceContext& context, D3D11Mesh const& mesh, D3D11Material const& material) {
    mesh.Bind(context);
    _pso->Bind(context);
    context.DrawIndexed(mesh.GetIndexCount(), 0u, 0u);
}
