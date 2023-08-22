#pragma once

#include "D3D11SolidStrategy.h"
#include "D3D11SolidTexturedStrategy.h"
#include "D3D11PhongStrategy.h"
#include "D3D11PhongTexStrategy.h"
#include "D3D11PhongTexNormStrategy.h"
#include "D3D11EnvironmentMappingReflectStrategy.h"
#include "D3D11EnvironmentMappingNormalStrategy.h"
#include "D3D11BasicIBLStrategy.h"
#include "D3D11BasicIBLTexturedStrategy.h"

#include "Graphics/D3D11/ShaderResource/D3D11ShaderResource.h"

namespace Engine::Graphics {
    // Initialize Render Strategies
    void InitRenderStrategies();

    //inline x_vector<std::function<void(ID3D11DeviceContext&, D3D11Mesh const&, D3D11Material const&)>> renderStrategyTable {
    //    &D3D11SolidStrategy::Render,
    //    &D3D11SolidTexturedStrategy::Render,
    //    &D3D11PhongStrategy::Render,
    //    &D3D11PhongTexStrategy::Render,
    //    &D3D11EnvironmentMappingNormalStrategy::Render,
    //    &D3D11EnvironmentMappingReflectStrategy::Render,
    //    &D3D11BasicIBLStrategy::Render,
    //    &D3D11BasicIBLTexturedStrategy::Render,
    //};

    // Misc Effects                             | Textures           | Shading Method
    // 00000000000000000000000000000000000000000'00000000000000000000'000,
    inline static x_unordered_map<uint64_t, std::function<void(ID3D11DeviceContext&, D3D11Mesh const&, D3D11Material const&)>> renderStrategyTable {
        {0b00000000000000000000000000000000000000000'00000000000000000000'000, &D3D11SolidStrategy::Render},
        {0b00000000000000000000000000000000000000000'00000000000000000010'000, &D3D11SolidTexturedStrategy::Render},
        {0b00000000000000000000000000000000000000000'00000000000000000000'001, &D3D11PhongStrategy::Render},
        {0b00000000000000000000000000000000000000000'00000000000000000010'001, &D3D11PhongTexStrategy::Render},
        {0b00000000000000000000000000000000000000000'00000000000000001010'001, &D3D11PhongTexNormStrategy::Render},
    };
}

