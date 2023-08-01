#pragma once

#include <variant>

namespace Engine::Graphics {
    class DummyEnd;

#define SHADER_RESOURCE_TYPES \
    F(D3D11DiffuseTexture)    \
    F(D3D11NormalMap)         \
    F(D3D11SpecularMap)       

    // Forward Declarations
    #define F(x) class x;
        SHADER_RESOURCE_TYPES
    #undef F

    // Variant Definition
    #define F(x) x##,
        using D3D11ShaderResource = std::variant<SHADER_RESOURCE_TYPES DummyEnd>;
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
}

