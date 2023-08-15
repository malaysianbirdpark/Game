#pragma once

#include <variant>

#include "D3D11SolidStrategy.h"
#include "D3D11TexturedStrategy.h"
#include "D3D11PhongTexStrategy.h"

namespace Engine::Graphics {
#define RENDER_STRATEGIES                   \
    F(D3D11SolidStrategy)                   \
    F(D3D11TexturedStrategy)                \
    F(D3D11PhongTexStrategy)

    // Initialize Render Strategies
    void InitRenderStrategies();

    // Variant Definition
    #define F(x) x##,
        using D3D11RenderStrategy = std::variant<D3D11SolidStrategy, D3D11TexturedStrategy, D3D11PhongTexStrategy>;
    #undef F

    // Bind Declarations
#define F(x) void Render##x(ID3D11DeviceContext& context, x const& strategy, class D3D11Mesh* mesh, class D3D11Material* material);
        RENDER_STRATEGIES
    #undef F

    struct StrategicRender {
            StrategicRender(ID3D11DeviceContext& context, D3D11Mesh* mesh, D3D11Material* material) : _context{&context}, _mesh{mesh}, _material{material} {}
    #define F(x) void operator() (x const& target) { Render##x(*_context, target, _mesh, _material); } 
        RENDER_STRATEGIES
    #undef F
    private:
        ID3D11DeviceContext*   _context;
        D3D11Mesh*             _mesh;
        D3D11Material*         _material;
    };

    D3D11RenderStrategy ResolveRenderStrategy(x_string const& tag);

    inline static x_unordered_map<x_string, std::function<D3D11RenderStrategy(void)>> RenderStrategyTable {
        {"solid", &D3D11SolidStrategy::GetInstance},
        {"textured", &D3D11TexturedStrategy::GetInstance},
        {"phong_tex", &D3D11PhongTexStrategy::GetInstance}
    };
}

