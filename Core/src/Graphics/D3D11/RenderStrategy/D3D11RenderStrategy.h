#pragma once

#include <variant>

namespace Engine::Graphics {
    class DummyEnd;

#define RENDER_STRATEGIES                   \
    F(D3D11SolidStrategy)                   \
    F(D3D11TexturedStrategy)                
    //F(D3D11BlinPhongSolidStrategy)          \
    //F(D3D11BlinPhongTexturedStrategy)       \
    //F(D3D11BlinPhongNormMapStrategy)        \
    //F(D3D11BlinPhongNormSpecMapStrategy)      

    // Forward Declarations
    #define F(x) class x;
        RENDER_STRATEGIES
    #undef F

    // Initialize Render Strategies
    void InitRenderStrategies();

    // Variant Definition
    #define F(x) x##,
        using D3D11RenderStrategy = std::variant<RENDER_STRATEGIES DummyEnd>;
    #undef F

    // Bind Declarations
    #define F(x) void Render##x(ID3D11DeviceContext& context, x const& strategy, class D3D11Mesh* mesh);
        RENDER_STRATEGIES
    #undef F

    struct StrategicRender {
            StrategicRender(ID3D11DeviceContext& context, D3D11Mesh* mesh) : _context{context}, _mesh{mesh} {}
    #define F(x) void operator() (x const& target) { Render##x(_context, target, _mesh); } 
        RENDER_STRATEGIES
    #undef F
    private:
        ID3D11DeviceContext&  _context;
        D3D11Mesh*            _mesh;
    };
}
