#pragma once

#include <variant>

#include "D3D11EmissiveMap.h"
#include "D3D11DiffuseMap.h"
#include "D3D11SpecularMap.h"
#include "D3D11NormalMap.h"
#include "D3D11HeightMap.h"
#include "D3D11OpacityMap.h"
#include "D3D11DiffuseCubemapTexture.h"
#include "D3D11SpecularCubemapTexture.h"

namespace Engine::Graphics {
#define SHADER_RESOURCE_TYPES       \
    F(D3D11EmissiveMap)             \
    F(D3D11DiffuseMap)              \
    F(D3D11SpecularMap)             \
    F(D3D11NormalMap)               \
    F(D3D11HeightMap)               \
    F(D3D11OpacityMap)              \
    F(D3D11DiffuseCubemapTexture)   \
    F(D3D11SpecularCubemapTexture) 

    // Variant Definition
    using D3D11ShaderResource = 
        std::variant<
            std::shared_ptr<D3D11EmissiveMap>,
            std::shared_ptr<D3D11DiffuseMap>,
            std::shared_ptr<D3D11SpecularMap>,
            std::shared_ptr<D3D11NormalMap>,
            std::shared_ptr<D3D11HeightMap>,
            std::shared_ptr<D3D11OpacityMap>,
            std::shared_ptr<D3D11DiffuseCubemapTexture>,
            std::shared_ptr<D3D11SpecularCubemapTexture>
        >;

    // Bind Declarations
    #define F(x) inline void Bind##x(ID3D11DeviceContext& context, x const& target);
        SHADER_RESOURCE_TYPES
    #undef F

    struct BindShaderResource {
        BindShaderResource(ID3D11DeviceContext& context) : _context{context} {}
    #define F(x) void operator() (std::shared_ptr<x> const& target) { target->Bind(_context); } 
        SHADER_RESOURCE_TYPES
    #undef F
    private:
        ID3D11DeviceContext& _context;
    };

    enum class ShaderResourceTypes {
        EmissiveMap = 3,
        DiffuseMap,
        SpecularMap,             
        NormalMap,             
        HeightMap,               
        OpacityMap,              
        DiffuseCubemapTexture,
        SpecularCubemapTexture,
    };

    class D3D11ShaderResourceHolder {
    public:
        [[nodiscard]] static D3D11ShaderResource Resolve(ID3D11Device& device, ShaderResourceTypes type, char const* path);
    private:
        inline static x_vector<std::function<x_string(char const*)>> GenUIDTable {
            &D3D11EmissiveMap::GenUID,
            &D3D11DiffuseMap::GenUID,
            &D3D11SpecularMap::GenUID,
            &D3D11NormalMap::GenUID,
            &D3D11HeightMap::GenUID,
            &D3D11OpacityMap::GenUID,
            &D3D11DiffuseCubemapTexture::GenUID,
            &D3D11SpecularCubemapTexture::GenUID
        };
        inline static x_vector<std::function<D3D11ShaderResource(ID3D11Device&, char const*)>> ConstructorTable {
            &D3D11EmissiveMap::CreateEmissiveMap,
            &D3D11DiffuseMap::CreateDiffuseMap,
            &D3D11SpecularMap::CreateSpecularMap,
            &D3D11NormalMap::CreateNormalMap,
            &D3D11HeightMap::CreateHeightMap,
            &D3D11OpacityMap::CreateOpacityMap,
            &D3D11DiffuseCubemapTexture::CreateDiffuseCubemapTexture,
            &D3D11SpecularCubemapTexture::CreateSpecularCubemapTexture,
        };
        inline static x_unordered_map<x_string, D3D11ShaderResource> _srs;
    };

}

