#pragma once

#include <variant>

#include "D3D11EmissiveMap.h"
#include "D3D11DiffuseMap.h"
#include "D3D11SpecularMap.h"
#include "D3D11NormalMap.h"
#include "D3D11HeightMap.h"
#include "D3D11OpacityMap.h"

namespace Engine::Graphics {
#define SHADER_RESOURCE_TYPES \
    F(D3D11EmissiveMap)       \
    F(D3D11DiffuseMap)        \
    F(D3D11SpecularMap)       \
    F(D3D11NormalMap)         \
    F(D3D11HeightMap)         \
    F(D3D11OpacityMap)

    // Variant Definition
    #define F(x) x##,
        using D3D11ShaderResource = 
            std::variant<
                D3D11EmissiveMap,
                D3D11DiffuseMap,
                D3D11SpecularMap,
                D3D11NormalMap,
                D3D11HeightMap,
                D3D11OpacityMap
            >;
    #undef F

    // Bind Declarations
    #define F(x) void Bind##x(ID3D11DeviceContext& context, x const& target);
        SHADER_RESOURCE_TYPES
    #undef F

    struct BindShaderResource {
        BindShaderResource(ID3D11DeviceContext& context) : _context{context} {}
    #define F(x) void operator() (x const& target) { Bind##x(_context, target); } 
        SHADER_RESOURCE_TYPES
    #undef F
    private:
        ID3D11DeviceContext& _context;
    };

    D3D11ShaderResource ResolveShaderResource(ID3D11Device& device, x_string const& tag, char const* path = "");

    inline static x_unordered_map<x_string, std::function<D3D11ShaderResource(ID3D11Device&, char const*)>> ShaderResourceTable {
        {"emissive_map", &D3D11EmissiveMap::CreateEmissiveMap},
        {"diffuse_map", &D3D11DiffuseMap::CreateDiffuseMap},
        {"specular_map", &D3D11SpecularMap::CreateSpecularMap},
        {"normal_map", &D3D11NormalMap::CreateNormalMap},
        {"height_map", &D3D11HeightMap::CreateHeightMap},
        {"opacity_map", &D3D11OpacityMap::CreateOpacityMap}
    };
}

