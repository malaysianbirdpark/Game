#pragma once

#include <variant>

#include "D3D11GlobalConstants.h"
#include "D3D11HemisphericAmbientLight.h"
#include "D3D11DirectionalLight.h"
#include "D3D11PointLight.h"

namespace Engine::Graphics {
#define CONSTANT_BUFFER_TYPES       \
    F(D3D11GlobalConstants)         \
    F(D3D11HemisphericAmbientLight) \
    F(D3D11DirectionalLight)        \
    F(D3D11PointLight)

    // Forward Declarations
    #define F(x) class x;
        CONSTANT_BUFFER_TYPES
    #undef F

    // Variant Definition
    #define F(x) x##,
            using D3D11ConstantBuffer = 
            std::variant<
                D3D11GlobalConstants,
                D3D11DirectionalLight,
                D3D11HemisphericAmbientLight,
                D3D11PointLight
            >;
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
        UpdateConstantBuffer(float const dt) : _dt{dt} {}
    #define F(x) void operator() (x& target) { target.Update(_dt); } 
        CONSTANT_BUFFER_TYPES
    #undef F
    private:
        float _dt;
    };

    struct UpdateLightPos {
        UpdateLightPos(DirectX::XMFLOAT3&& pos) : _pos{std::move(pos)} {}
        void operator() (D3D11GlobalConstants& target) {}
        void operator() (D3D11DirectionalLight& target) {}
        void operator() (D3D11HemisphericAmbientLight& target) {}
        void operator() (D3D11PointLight& target) { target.SetPosition(_pos); }
    private:
        DirectX::XMFLOAT3 _pos;
    };
}

