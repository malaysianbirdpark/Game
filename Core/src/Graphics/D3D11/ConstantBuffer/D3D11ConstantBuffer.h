#pragma once

#include <variant>

#include "D3D11CamPosition.h"
#include "D3D11LightDirectional.h"

namespace Engine::Graphics {
#define CONSTANT_BUFFER_TYPES \
    F(D3D11CamPosition)       \
    F(D3D11LightDirectional)  

    // Forward Declarations
    #define F(x) class x;
        CONSTANT_BUFFER_TYPES
    #undef F

    // Variant Definition
    #define F(x) x##,
        using D3D11ConstantBuffer = std::variant<D3D11CamPosition, D3D11LightDirectional>;
    #undef F

    // Bind Declarations
    #define F(x) void Bind##x(ID3D11DeviceContext& context, x& target);
        CONSTANT_BUFFER_TYPES
    #undef F

    struct BindConstantBuffer {
        BindConstantBuffer(ID3D11DeviceContext& context) : _context{context} {}
    #define F(x) void operator() (x& target) { target.Bind(_context); } 
        CONSTANT_BUFFER_TYPES
    #undef F
    private:
        ID3D11DeviceContext& _context;
    };

    // Update Declarations
    #define F(x) void Update##x(x& target);
        CONSTANT_BUFFER_TYPES
    #undef F

    struct UpdateConstantBuffer {
    #define F(x) void operator() (x& target) { target.Update(); } 
        CONSTANT_BUFFER_TYPES
    #undef F
    private:
    };
}

