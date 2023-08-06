#include "pch.h"
#include "D3D11ShaderResource.h"

void Engine::Graphics::BindD3D11DiffuseMap(ID3D11DeviceContext& context, D3D11DiffuseMap const& target) {
    target.Bind(context);
}

void Engine::Graphics::BindD3D11NormalMap(ID3D11DeviceContext& context, D3D11NormalMap const& target) {
    target.Bind(context);
}

void Engine::Graphics::BindD3D11SpecularMap(ID3D11DeviceContext& context, D3D11SpecularMap const& target) {
    target.Bind(context);
}

Engine::Graphics::D3D11ShaderResource Engine::Graphics::ResolveShaderResource(ID3D11Device& device, x_string const& tag, char const* path) {
    x_string str_path {path};
    return ShaderResourceTable[tag](device, path);
}




