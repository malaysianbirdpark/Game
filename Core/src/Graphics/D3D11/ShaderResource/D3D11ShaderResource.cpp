#include "pch.h"
#include "D3D11ShaderResource.h"

#include "D3D11DiffuseTexture.h"
#include "D3D11NormalMap.h"
#include "D3D11SpecularMap.h"

void Engine::Graphics::BindD3D11DiffuseTexture(ID3D11DeviceContext& context, D3D11DiffuseTexture const& target) {
    target.Bind(context);
}

void Engine::Graphics::BindD3D11NormalMap(ID3D11DeviceContext& context, D3D11NormalMap const& target) {
    target.Bind(context);
}

void Engine::Graphics::BindD3D11SpecularMap(ID3D11DeviceContext& context, D3D11SpecularMap const& target) {
    target.Bind(context);
}




