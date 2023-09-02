#include "pch.h"
#include "D3D11Cubemap.h"

#include "D3D11RenderCommand.h"
#include "Graphics/D3DCamera.h"
#include "Graphics/D3D11/PipelineState/D3D11PSOLibrary.h"
#include "Graphics/D3D11/PipelineState/D3D11PipelineStateObject.h"

Engine::Graphics::D3D11Cubemap::D3D11Cubemap(ID3D11Device& device, ID3D11DeviceContext& context, DirectX::XMMATRIX proj)
    : _vertex{device, sizeof(CubeMapVertex), sizeof(CubeMapVertex) * sizeof(cube_vertices), cube_vertices.data(), "cubemap"},
      _index{device, cube_indices.data(), cube_indices.size(), "cubemap"},
      _transform{device, context, DirectX::XMMatrixIdentity()}
{
    _pso = D3D11PSOLibrary::ResolvePSO("cubemap");

    GRAPHICS_INFO("Loading Cubemaps..");
    //AddSDRTexture(device, context, "Assets/CubeMapTextures/SantaMaria.dds");
    //AddSDRTexture(device, context, "Assets/CubeMapTextures/FortPoint.dds"); 
    //AddSDRTexture(device, context, "Assets/CubeMapTextures/Atrium_specularIBL.dds", "Assets/CubeMapTextures/Atrium_diffuseIBL.dds");
    //AddSDRTexture(device, context, "Assets/CubeMapTextures/Garage_specularIBL.dds", "Assets/CubeMapTextures/Garage_diffuseIBL.dds");
    AddHDRTexture(
        device, 
        context,
        "Assets/CubeMapTextures/Bridge/BridgeEnvHDR.dds",
        "Assets/CubeMapTextures/Bridge/BridgeSpecularHDR.dds",
        "Assets/CubeMapTextures/Bridge/BridgeDiffuseHDR.dds",
        "Assets/CubeMapTextures/Bridge/BridgeBrdf.dds"
    );
    AddHDRTexture(
        device, 
        context,
        "Assets/CubeMapTextures/IBLBakerDefault/IBLBakerDefaultEnvHDR.dds",
        "Assets/CubeMapTextures/IBLBakerDefault/IBLBakerDefaultSpecularHDR.dds",
        "Assets/CubeMapTextures/IBLBakerDefault/IBLBakerDefaultDiffuseHDR.dds",
        "Assets/CubeMapTextures/IBLBakerDefault/IBLBakerDefaultBrdf.dds"
    );
    AddHDRTexture(
        device,
        context,
        "Assets/CubeMapTextures/Countryside/CountrysideEnvHDR.dds",
        "Assets/CubeMapTextures/Countryside/CountrysideSpecularHDR.dds",
        "Assets/CubeMapTextures/Countryside/CountrysideDiffuseHDR.dds",
        "Assets/CubeMapTextures/Countryside/CountrysideBrdf.dds"
    );
    AddHDRTexture(
        device,
        context,
        "Assets/CubeMapTextures/Indoor/IndoorEnvHDR.dds",
        "Assets/CubeMapTextures/Indoor/IndoorSpecularHDR.dds",
        "Assets/CubeMapTextures/Indoor/IndoorDiffuseHDR.dds",
        "Assets/CubeMapTextures/Indoor/IndoorBrdf.dds"
    );
    AddHDRTexture(
        device,
        context,
        "Assets/CubeMapTextures/Bench/BenchEnvHDR.dds",
        "Assets/CubeMapTextures/Bench/BenchSpecularHDR.dds",
        "Assets/CubeMapTextures/Bench/BenchDiffuseHDR.dds",
        "Assets/CubeMapTextures/Bench/BenchBrdf.dds"
    );
    GRAPHICS_INFO("Loading Cubemaps.. done");
}

void Engine::Graphics::D3D11Cubemap::AddSDRTexture(ID3D11Device& device, ID3D11DeviceContext& context, char const* specular_path) {
    _textures.push_back(D3D11SDRCubemapTexture{device, context, specular_path});
}

void Engine::Graphics::D3D11Cubemap::AddSDRTexture(ID3D11Device& device, ID3D11DeviceContext& context, char const* specular_path, char const* diffuse_path) {
    _textures.push_back(D3D11SDRCubemapTexture{device, context, specular_path, specular_path, diffuse_path});
}

void Engine::Graphics::D3D11Cubemap::AddHDRTexture(ID3D11Device& device, ID3D11DeviceContext& context, char const* env_path, char const* specular_path, char const* diffuse_path) {
    _textures.push_back(D3D11HDRCubemapTexture{device, context, env_path, specular_path, diffuse_path});
}

void Engine::Graphics::D3D11Cubemap::AddHDRTexture(ID3D11Device& device, ID3D11DeviceContext& context, char const* env_path, char const* specular_path, char const* diffuse_path, char const* brdf_path) {
    _textures.push_back(D3D11HDRCubemapTexture{device, context, env_path, specular_path, diffuse_path, brdf_path});
}

void Engine::Graphics::D3D11Cubemap::SetTexture(uint8_t idx) {
    CORE_ASSERT(idx < _textures.size(), "Provided Cubemap texture index is invalid");
    _selectedTexture = idx;
}

int& Engine::Graphics::D3D11Cubemap::TextureIndex() {
    return _selectedTexture;
}

void Engine::Graphics::D3D11Cubemap::Update(float const dt) {
}

void Engine::Graphics::D3D11Cubemap::Bind(ID3D11DeviceContext& context) {
    std::visit(BindCubemapTexture{context}, _textures[_selectedTexture]);
}

void Engine::Graphics::D3D11Cubemap::Render(ID3D11DeviceContext& context) {
    context.IASetPrimitiveTopology(_topology);
    _vertex.Bind(context);
    _index.Bind(context);

    _pso->Bind(context);
    Bind(context);

    _transform.Update(context);
    _transform.Bind(context);

    context.DrawIndexed(cube_indices.size(), 0u, 0u);
}
