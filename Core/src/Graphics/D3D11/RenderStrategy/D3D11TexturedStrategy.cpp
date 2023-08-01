#include "pch.h"
#include "D3D11TexturedStrategy.h"

#include "Graphics/D3D11/PipelineState/D3D11PSOLibrary.h"
#include "Graphics/D3D11/PipelineState/D3D11PipelineStateObject.h"

#include "Graphics/D3D11/SceneGraph/D3D11SceneGraph.h"

#include "Graphics/D3D11/ShaderResource/D3D11ShaderResourcesInclude.h"

void Engine::Graphics::D3D11TexturedStrategy::Init() {
    _pso = D3D11PSOLibrary::ResolvePSO("textured");
}

void Engine::Graphics::D3D11TexturedStrategy::Render(ID3D11DeviceContext& context, D3D11Mesh const& mesh) const {
    _pso->Bind(context);

    for (auto const& sr : mesh.GetShaderResources()) {
        if (D3D11DiffuseTexture const* diffuse_texture {std::get_if<D3D11DiffuseTexture>(sr.get())}) {
            diffuse_texture->Bind(context);  
        }
    }

    context.DrawIndexed(mesh.GetIndexCount(), 0u, 0u);
}
