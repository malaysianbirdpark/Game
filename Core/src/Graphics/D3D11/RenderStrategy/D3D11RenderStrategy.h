#pragma once

#include "D3D11SolidStrategy.h"
#include "D3D11SolidTexStrategy.h"
#include "D3D11SolidTexHeightStrategy.h"
#include "D3D11PhongStrategy.h"
#include "D3D11PhongTexStrategy.h"
#include "D3D11PhongTexNormStrategy.h"
#include "D3D11PhongTexNormSpecStrategy.h"
#include "D3D11EnvironmentMappingReflectStrategy.h"
#include "D3D11EnvironmentMappingNormalStrategy.h"
#include "D3D11BasicIBLStrategy.h"
#include "D3D11BasicIBLTexturedStrategy.h"

#include "Graphics/D3D11/ShaderResource/D3D11ShaderResource.h"

namespace Engine::Graphics {
    // Initialize Render Strategies
    void InitRenderStrategies();

    // Misc Effects                             | Textures           | Shading Method
    // 00000000000000000000000000000000000000000'00000000000000000000'000,
    inline static x_unordered_map<uint64_t, std::function<void(ID3D11DeviceContext&, D3D11Mesh const&, D3D11Material&)>> renderStrategyTable {
        {0b00000000000000000000000000000000000000000'00000000000000000000'000, &D3D11SolidStrategy::Render},
        {0b00000000000000000000000000000000000000000'00000000000000000010'000, &D3D11SolidTexStrategy::Render},
        {0b00000000000000000000000000000000000000000'00000000000000010010'000, &D3D11SolidTexHeightStrategy::Render},
        {0b00000000000000000000000000000000000000000'00000000000000000000'001, &D3D11PhongStrategy::Render},
        {0b00000000000000000000000000000000000000000'00000000000000000010'001, &D3D11PhongTexStrategy::Render},
        {0b00000000000000000000000000000000000000000'00000000000000001010'001, &D3D11PhongTexNormStrategy::Render},
        {0b00000000000000000000000000000000000000000'00000000000000001110'001, &D3D11PhongTexNormSpecStrategy::Render},
    };
}

