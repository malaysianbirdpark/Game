#include "pch.h"
#include "D3D11BasicIBLTexturedStrategy.h"

#include "Graphics/D3D11/PipelineState/D3D11PSOLibrary.h"
#include "Graphics/D3D11/PipelineState/D3D11PipelineStateObject.h"

#include "Graphics/D3D11/SceneGraph/D3D11SceneGraph.h"

#include "Graphics/D3D11/D3D11CubeMap.h"

void Engine::Graphics::D3D11BasicIBLTexturedStrategy::Init() {
    GRAPHICS_INFO("Basic IBL Strategy Init()");
    _pso = D3D11PSOLibrary::ResolvePSO("basic_ibl_textured");
}

void Engine::Graphics::D3D11BasicIBLTexturedStrategy::Render(ID3D11DeviceContext& context, D3D11Mesh const& mesh, D3D11Material const& material) {
    mesh.Bind(context);
    _pso->Bind(context);

    D3D11CubeMap::BindDiffuse(context);
    D3D11CubeMap::BindSpecular(context);

    for (auto const& sr : material.GetShaderResources()) {
        if (auto const* diffuse_texture {std::get_if<std::shared_ptr<D3D11DiffuseMap>>(&sr)}) {
            diffuse_texture->get()->Bind(context);  
        }
    }

    context.DrawIndexed(mesh.GetIndexCount(), 0u, 0u);
}

