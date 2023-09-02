#include "pch.h"
#include "D3D11UnrealPBRStrategy.h"

#include "Graphics/D3D11/D3D11Cubemap.h"
#include "Graphics/D3D11/PipelineState/D3D11PSOLibrary.h"
#include "Graphics/D3D11/PipelineState/D3D11PipelineStateObject.h"

#include "Graphics/D3D11/SceneGraph/D3D11SceneGraph.h"

void Engine::Graphics::D3D11UnrealPBRStrategy::Init() {
    GRAPHICS_INFO("PBR Strategy Init()");
    _pso = D3D11PSOLibrary::ResolvePSO("unreal_pbr");
}

void Engine::Graphics::D3D11UnrealPBRStrategy::Render(ID3D11DeviceContext& context, D3D11Mesh const& mesh, D3D11Material& material) {
    mesh.Bind(context);
    material.BindUnrealPBR(context);
    material.BindVertexShaderConstants(context);
    D3D11Cubemap::Bind(context);
    _pso->Bind(context);

    for (auto const& sr : material.GetShaderResources()) {
        if (auto const* emissive_map {std::get_if<std::shared_ptr<D3D11EmissiveMap>>(&sr)}) {
            emissive_map->get()->Bind(context);  
        }
        else if (auto const* diffuse_map {std::get_if<std::shared_ptr<D3D11DiffuseMap>>(&sr)}) {
            diffuse_map->get()->Bind(context);  
        }
        else if (auto const* specular_map {std::get_if<std::shared_ptr<D3D11SpecularMap>>(&sr)}) {
            specular_map->get()->Bind(context);  
        }
        else if (auto const* normal_map {std::get_if<std::shared_ptr<D3D11NormalMap>>(&sr)}) {
            normal_map->get()->Bind(context);  
        }
        else if (auto const* height_map {std::get_if<std::shared_ptr<D3D11HeightMap>>(&sr)}) {
            height_map->get()->Bind(context);  
        }
        else if (auto const* metallic_map {std::get_if<std::shared_ptr<D3D11MetallicMap>>(&sr)}) {
            metallic_map->get()->Bind(context);  
        }
        else if (auto const* roughness_map {std::get_if<std::shared_ptr<D3D11RoughnessMap>>(&sr)}) {
            roughness_map->get()->Bind(context);  
        }
        else if (auto const* ao_map {std::get_if<std::shared_ptr<D3D11AmbientOcclusionMap>>(&sr)}) {
            ao_map->get()->Bind(context);  
        }
    }

    context.DrawIndexed(mesh.GetIndexCount(), 0u, 0u);
}

