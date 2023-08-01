#pragma once

#include <variant>

namespace Engine::Graphics {
    class DummyEnd;

#define CONSTANT_BUFFER_TYPES \
    F(D3D11TransformMVP)      \
    F(D3D11LightDirectional)  \

    // Forward Declarations
    #define F(x) class x;
        CONSTANT_BUFFER_TYPES
    #undef F

    // Variant Definition
    #define F(x) x##,
        using D3D11ConstantBuffer = std::variant<CONSTANT_BUFFER_TYPES DummyEnd>;
    #undef F

    // Bind Declarations
    #define F(x) void Bind##x(ID3D11DeviceContext& context, x const& target);
        CONSTANT_BUFFER_TYPES
    #undef F

    struct BindConstantBuffer {
        BindConstantBuffer(ID3D11DeviceContext& context) : _context{context} {}
    #define F(x) void operator() (x const& target) { Bind##x(_context, target); } 
        CONSTANT_BUFFER_TYPES
    #undef F
    private:
        ID3D11DeviceContext& _context;
    };
}

