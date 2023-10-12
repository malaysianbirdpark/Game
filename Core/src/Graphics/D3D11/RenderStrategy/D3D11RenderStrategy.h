#pragma once

#include "D3D11UnrealPBRStrategy.h"
#include "D3D11PhongStrategy.h"
#include "D3D11SolidStrategy.h"

#include "..\ShaderResource\D3D11ShaderResource.h"

namespace Engine::Graphics {
    using RenderStrategy = std::function<void(ID3D11DeviceContext&, class D3D11Mesh const&, class D3D11Material&)>;

    enum class RenderStrategyType {
        Solid = 0,
        Phong,
        UnrealPBR,
    };

    // Initialize Render Strategies
    void InitRenderStrategies();

    inline static x_unordered_map<int32_t, RenderStrategy> renderStrategyTable {
        {0, &D3D11SolidStrategy::Render},
        {1, &D3D11PhongStrategy::Render},
        {2, &D3D11UnrealPBRStrategy::Render},
    };
}

