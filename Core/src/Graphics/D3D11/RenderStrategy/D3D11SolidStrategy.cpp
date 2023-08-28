#include "pch.h"
#include "D3D11SolidStrategy.h"

#include "Graphics/D3D11/SceneGraph/D3D11SceneGraph.h"
#include "Graphics/D3D11/PipelineState/D3D11PSOLibrary.h"
#include "Graphics/D3D11/PipelineState/D3D11PipelineStateObject.h"

void Engine::Graphics::D3D11SolidStrategy::Init() {
    _pso = D3D11PSOLibrary::ResolvePSO("solid");
}

void Engine::Graphics::D3D11SolidStrategy::Render(ID3D11DeviceContext& context, D3D11Mesh const& mesh, D3D11Material& material) {
    mesh.Bind(context);
    material.BindSolid(context);
    material.BindVertexShaderConstants(context);
    _pso->Bind(context);

    for (auto const& sr : material.GetShaderResources()) {
        if (auto const* diffuse_texture {std::get_if<std::shared_ptr<D3D11DiffuseMap>>(&sr)}) {
            diffuse_texture->get()->Bind(context);  
        }
        if (auto const* height_map {std::get_if<std::shared_ptr<D3D11HeightMap>>(&sr)}) {
            height_map->get()->Bind(context);  
        }
    }

    context.DrawIndexed(mesh.GetIndexCount(), 0u, 0u);
}
